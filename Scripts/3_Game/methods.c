bool gl_OnlineServicesInit = false;

string GetBIOSStoredSteam64() {
    if(GetGame().IsServer() || !GetGame().IsMultiplayer()) return "-1";

    if(!gl_OnlineServicesInit) {
        OnlineServices.Init();
        gl_OnlineServicesInit = true;
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