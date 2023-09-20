//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : ScriptedDamageManagerComponent
{
	//-----------------------------------------------------------------------------------------------------------
	protected override void OnDamage(EDamageType type, float damage, HitZone pHitZone, IEntity instigator, inout vector hitTransform[3], float speed, int colliderID, int nodeID)
	{
		//PrintFormat("DamageManager OnDamage HitZone: %1 (%4) Damage: %2 Type: %3", pHitZone.GetName(), damage, type, pHitZone.GetHealth());
		
		if(GetHealthScaled() < 0.01)
		{
			//PrintFormat("Health HZ reaches 1% threshold: %1", GetHealthScaled());
			
			GetDefaultHitZone().SetHealthScaled(0.01);
			
			//Print("Set Health to 1%");
		}
		
		super.OnDamage(type, damage, pHitZone, instigator, hitTransform, speed, colliderID, nodeID);
	}
};
