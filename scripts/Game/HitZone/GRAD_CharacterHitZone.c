//------------------------------------------------------------------------------------------------
modded class SCR_RegeneratingHitZone : ScriptedHitZone
{
	//-----------------------------------------------------------------------------------------------------------
	override void OnDamage(notnull BaseDamageContext damageContext)
	{
		//PrintFormat("HitZone OnDamage HitZone (Direct): %1 (%4) Damage: %2 Type: %3", damageContext.struckHitZone.GetName(), damageContext.damageValue, damageContext.damageType, damageContext.struckHitZone.GetHealth());

		IEntity hzOwner = GetOwner();
		if (hzOwner && EntityUtils.IsPlayer(hzOwner))
		{
			if(GetHealthScaled() < 0.01)
			{
				//PrintFormat("Health HZ reaches 1% threshold: %1", GetHealthScaled());
				
				SetHealthScaled(0.01);
				
				//Print("Set Health to 1%");
			}
		}
		
		super.OnDamage(damageContext);
	}
};
