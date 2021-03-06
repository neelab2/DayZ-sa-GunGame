// Quick version should do the magic
//modded class FirearmActionAttachMagazine 
modded class FirearmActionAttachMagazineQuick
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item ) //condition for action
	{
		Weapon_Base weapon = Weapon_Base.Cast(player.GetHumanInventory().GetEntityInHands());
		int mi = weapon.GetCurrentMuzzle();
		Magazine mag = weapon.GetMagazine(mi);
		
		return !mag.IsFullQuantity();
	}
	
	override void Start( ActionData action_data )
	{
		//super.Start( action_data );
		Weapon_Base weapon = Weapon_Base.Cast(action_data.m_MainItem);
		int mi = weapon.GetCurrentMuzzle();
		Magazine mag = weapon.GetMagazine(mi);

		mag.LocalSetAmmoMax();
		if(weapon.IsChamberEmpty(weapon.GetCurrentMuzzle()))
			pushToChamberFromAttachedMagazine( weapon, weapon.GetCurrentMuzzle() );

		action_data.m_Player.GetWeaponManager().SwapMagazine(mag, this);
	}
}