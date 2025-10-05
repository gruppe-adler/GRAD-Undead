//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : ScriptedDamageManagerComponent
{
	//-----------------------------------------------------------------------------------------------------------
	protected override void OnDamage(notnull BaseDamageContext damageContext)
	{
		//PrintFormat("DamageManager OnDamage HitZone: %1 (%4) Damage: %2 Type: %3", damageContext.struckHitZone.GetName(), damageContext.damageValue, damageContext.damageType, damageContext.struckHitZone.GetHealth());

		// if damage type is 'collision' which could be for example fall damage
		// the the damage is also applied on all 6 leg parts to create treatable injuries
		
		if (damageContext.damageType == EDamageType.COLLISION)
		{
			array<HitZone> hitZones = {};
			GetAllHitZones(hitZones);
			
			foreach (HitZone hz : hitZones)
			{
				float fallDamage;
				
				switch (hz.GetName())
				{
					case "RThigh":
						fallDamage = damageContext.damageValue * 0.1;
						break;
					case "LThigh":
						fallDamage = damageContext.damageValue * 0.1;
						break;
					case "RCalf":
						fallDamage = damageContext.damageValue * 0.3;
						break;
					case "LCalf":
						fallDamage = damageContext.damageValue * 0.3;
						break;
					case "RFoot":
						fallDamage = damageContext.damageValue * 0.1;
						break;
					case "LFoot":
						fallDamage = damageContext.damageValue * 0.1;
						break;
					default:
						break;
				}
				
				float health = hz.GetHealth();
				
				float newHealth = health - fallDamage;
				
				if (newHealth <= 0)
					hz.SetHealthScaled(0.01);
				else
					hz.SetHealth(newHealth);
				
				// if the collision damage is above threshold then
				// the character gets unconscious for a certain amount of time
				
				// a damage of 25 is equal to a fall from a height of roughly 8-10 meters
				if (damageContext.damageValue > 25)
				{
					ChimeraCharacter character = ChimeraCharacter.Cast(GetOwner());
					if (!character)
					{
						super.OnDamage(damageContext);
						return;
					}
					
					CharacterControllerComponent controller = character.GetCharacterController();
					if (!controller)
					{
						super.OnDamage(damageContext);
						return;
					}
		
					controller.SetUnconscious(true);
					
					// wake up after 15 sec
					GetGame().GetCallqueue().CallLater(controller.SetUnconscious, 15000, false, false);
				}
			}
		}
		
		super.OnDamage(damageContext);
		
		// Prevent death by clamping ALL hit zones to minimum 1% after all damage processing
		IEntity hzOwner = GetOwner();
		if (hzOwner && EntityUtils.IsPlayer(hzOwner))
		{
			// Clamp all hit zones to prevent death
			array<HitZone> allHitZones = {};
			GetAllHitZones(allHitZones);
			
			foreach (HitZone hz : allHitZones)
			{
				if (hz.GetHealthScaled() < 0.01)
				{
					//PrintFormat("Clamping hitzone %1 from %2 to 1%%", hz.GetName(), hz.GetHealthScaled());
					hz.SetHealthScaled(0.01);
				}
			}
		}
	}
};
