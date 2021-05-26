string GetBIOSStoredSteam64() {
    if(!m_OnlineServicesInit) {
        OnlineServices.Init();
        m_OnlineServicesInit = true;
    }

    BiosUserManager user_manager = GetGame().GetUserManager();
    if(user_manager) {
        if(user_manager.GetTitleInitiator()) {
            user_manager.SelectUser( user_manager.GetTitleInitiator() );
        }
    }

    if(user_manager && user_manager.GetSelectedUser())
        return user_manager.GetSelectedUser().GetUid();

    return "-1";
};