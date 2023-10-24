//------------------------------------------------------------------------------------------------
modded class SCR_RegeneratingHitZone : ScriptedHitZone
{
	//-----------------------------------------------------------------------------------------------------------
	override void OnDamage(EDamageType type, float damage, HitZone pOriginalHitzone, notnull Instigator instigator, inout vector hitTransform[3], float speed, int colliderID, int nodeID)
	{
		//PrintFormat("HitZone OnDamage HitZone (Direct): %1 (%4) Damage: %2 Type: %3", pOriginalHitzone.GetName(), damage, type, pOriginalHitzone.GetHealth());

		IEntity hzOwner = GetOwner();
		if (!hzOwner)
			return;
		
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(hzOwner);
		SCR_PlayerController playerController =  SCR_PlayerController.Cast(GetGame().GetPlayerManager().GetPlayerController(playerId));
		if (!playerController)
			return;

		//Print(string.Format("GRAD Undead - isPossessing: %1", playerController.IsPossessing()), LogLevel.NORMAL);

		if (EntityUtils.IsPlayer(hzOwner) && !playerController.IsPossessing())
		{
			if(GetHealthScaled() < 0.01)
			{
				//PrintFormat("Health HZ reaches 1% threshold: %1", GetHealthScaled());
				
				SetHealthScaled(0.01);
				
				//Print("Set Health to 1%");
			}
		}
		
		super.OnDamage(type, damage, pOriginalHitzone, instigator, hitTransform, speed, colliderID, nodeID);
	}
};
