#ifdef EXPANSIONMOD
#ifdef GAMELABSEXPDEFCHECK
modded class ExpansionGlobalChatModule {
    void AddChatMessage(ref ParamsReadContext ctx, PlayerIdentity sender, Object target) {
        ChatMessageEventParams data;

		Object parent;
		int partyID = -1;

		if(!m_PartyModule) m_PartyModule = ExpansionPartyModule.Cast( GetModuleManager().GetModule(ExpansionPartyModule) );
		if(!ctx.Read(data)) { return; }

		if(IsMissionHost()) {
			PlayerBase player = GetPlayerObjectByIdentity(sender);
			bool canSendMessage = true;
			string channelName = "";

			switch ( data.param1 )
			{
				case ExpansionChatChannels.CCAdmin:
					canSendMessage = GetPermissionsManager().HasPermission( "Admin.Chat", sender );
					channelName = "Admin";
					break;
				case ExpansionChatChannels.CCGlobal:
					canSendMessage = GetExpansionSettings().GetGeneral().EnableGlobalChat;
					channelName = "Global";
					break;
				case ExpansionChatChannels.CCTeam:
					if (!m_PartyModule) break;
					if (player) partyID = m_PartyModule.GetPartyID(player);

					canSendMessage = player && m_PartyModule.HasParty(player) && GetExpansionSettings().GetGeneral().EnablePartyChat;
					channelName = "Team";
					break;
				case ExpansionChatChannels.CCTransport:
					if (player) parent = Object.Cast( player.GetParent() );
					canSendMessage = parent && parent.IsTransport() && GetExpansionSettings().GetGeneral().EnableTransportChat;
					channelName = "Transport";
					break;
			}
			if ( canSendMessage )
			{
				data.param2 = sender.GetName();
				string steamid = sender.GetPlainId();
				string biuid = sender.GetId();
				string idtable = sender.GetPlayerId().ToString();

				if ( GetGame().IsMultiplayer() )
				{
					ScriptRPC rpc = new ScriptRPC();
					rpc.Write(data);

					if (parent)
						rpc.Write(parent);
					if (partyID >= 0)
						rpc.Write(partyID);

					rpc.Send(null, ExpansionGlobalChatRPC.AddChatMessage, true);

					_Payload_PlayerChat payload = new _Payload_PlayerChat(new _LogPlayer(player), data.param3);
					GetGameLabs().GetApi().PlayerChat(new _Callback(), _Payload_PlayerChat);
				}
				GetGame().AdminLog( "Chat[" + channelName + "](\"" + data.param2 + "\"(id=" + biuid + ")): " + data.param3 );
			} else {
				switch (data.param1)
				{
					case ExpansionChatChannels.CCTransport:
						if (!ctx.Read(parent))
							return;

						Object localParent = Object.Cast( g_Game.GetPlayer().GetParent() );
						if (parent.IsTransport() && localParent == parent)
							break;
						else
							return;

					break;

					case ExpansionChatChannels.CCTeam:
						if (!ctx.Read(partyID))
								return;

						if (partyID == m_PartyModule.GetPartyID())
							break;
						else
							return;

					break;
				}

				GetGame().GetMission().OnEvent(ChatMessageEventTypeID, data);
			}
		}
    }
};
#endif
#endif