//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : ScriptedDamageManagerComponent
{
	//-----------------------------------------------------------------------------------------------------------
	protected override void OnDamage(EDamageType type, float damage, HitZone pHitZone, IEntity instigator, inout vector hitTransform[3], float speed, int colliderID, int nodeID)
	{
		//PrintFormat("DamageManager OnDamage HitZone: %1 (%4) Damage: %2 Type: %3", pHitZone.GetName(), damage, type, pHitZone.GetHealth());
		
		ScriptedHitZone scriptedHz = ScriptedHitZone.Cast(pHitZone);
		if (!scriptedHz)
			return;

		IEntity hzOwner = scriptedHz.GetOwner();
		if (!hzOwner)
			return;
		
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(hzOwner);
		SCR_PlayerController playerController =  SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return;
		
		//Print(string.Format("GRAD Undead - isPossessing: %1", playerController.IsPossessing()), LogLevel.NORMAL);
		
		if (EntityUtils.IsPlayer(hzOwner) && !playerController.IsPossessing())
		{
			if(GetDefaultHitZone().GetHealthScaled() < 0.01)
			{
				//PrintFormat("Health HZ reaches 1% threshold: %1", GetHealthScaled());
				
				GetDefaultHitZone().SetHealthScaled(0.01);
				
				//Print("Set Health to 1%");
			}
		}

		// if damage type is 'collision' which could be for example fall damage
		// the the damage is also applied on all 6 leg parts to create treatable injuries
		
		if (type == EDamageType.COLLISION)
		{
			array<HitZone> hitZones = {};
			GetAllHitZones(hitZones);
			
			foreach (HitZone hitZone : hitZones)
			{
				float fallDamage;
				
				switch (hitZone.GetName())
				{
					case "RThigh":
						fallDamage = damage * 0.1;
						break;
					case "LThigh":
						fallDamage = damage * 0.1;
						break;
					case "RCalf":
						fallDamage = damage * 0.3;
						break;
					case "LCalf":
						fallDamage = damage * 0.3;
						break;
					case "RFoot":
						fallDamage = damage * 0.1;
						break;
					case "LFoot":
						fallDamage = damage * 0.1;
						break;
					default:
						break;
				}
				
				float health = hitZone.GetHealth();
				
				float newHealth = health - fallDamage;
				
				if (newHealth <= 0)
					hitZone.SetHealthScaled(0.01);
				else
					hitZone.SetHealth(newHealth);
				
				// if the collision damage is above threshold then
				// the character gets unconscious for a certain amount of time
				
				// a damage of 25 is equal to a fall from a height of roughly 8-10 meters
				if (damage > 25)
				{
					ChimeraCharacter character = ChimeraCharacter.Cast(GetOwner());
					if (!character)
						return;
					
					CharacterControllerComponent controller = character.GetCharacterController();
					if (!controller)
						return;
		
					controller.SetUnconscious(true);
					
					// wake up after 15 sec
					GetGame().GetCallqueue().CallLater(controller.SetUnconscious, 15000, false, false);
				}
			}
		}
		
		super.OnDamage(type, damage, pHitZone, instigator, hitTransform, speed, colliderID, nodeID);
	}
};
