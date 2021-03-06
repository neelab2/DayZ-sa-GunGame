class GunGameStateBase
{
    protected GunGameFSM FSM;

	void GunGameStateBase () 
    {}

    void SetFsmRef(GunGameFSM source)
    {
        if(FSM) return;
        FSM = source;
    }

	/**@fn		OnEntry
	 * @brief	called upon entry to state
	 * @NOTE	if state has (non-running) sub-machine, it's started on entry
	 * @param[in] e	the event that triggered transition to this state
	 **/
	void OnEntry (GunGameEventBase e)
    {
        Print("[FSM] OnEntry :" + this.ClassName());
    }

	/**@fn		OnUpdate
	 * @brief ongoing behavior, performed while being in the state
	 *
	 * @NOTE: this is supposed to be the Do() operation in UML speak
	 **/
	void OnUpdate (float dt);

	/**@fn		OnAbort
	 * @brief called when abort signal arrives
	 * @param[in] e	the event that triggered abort from this state
	 **/
	void OnAbort (GunGameEventBase e);

	/**@fn		OnExit
	 * @brief called on exit from state
	 * @param[in] e	the event that triggered transition from this state
	 **/
	void OnExit (GunGameEventBase e);

    int GetStateId()
    {
        return -1;
    }

    protected void ChangeState(GunGameEventBase e)
    {
        Print("ChangeState called");
        FSM.ProcessEvent(e);
    }
};

class GGWaitOnPlayers : GunGameStateBase
{	
    ref array<Man> m_Players = new array<Man>();

    override void OnUpdate (float dt)
    {
        GetGame().GetWorld().GetPlayerList(m_Players);
        if(m_Players.Count() >= DayZGame.gunGameconfig.MinPlayers)
        {
            ChangeState(new EnoughPlayersReadyEvent());
        }
    }
    override int GetStateId()
    {
        return 1;
    }
} 

class GGSelectMapState : GunGameStateBase
{
    float CountdownTimer;
    ref array<int> votes;
    int totalVotes;

    override void OnEntry (GunGameEventBase e)
    {
        super.OnEntry(e);
        CountdownTimer = 60;
        votes = new array<int>();
        int totalMapCount = LocationManager<GunGameLocation>.instance.GameModeMapCount();
        for (int i = 0; i < totalMapCount; i++)
        {
            votes.Insert(0);
        }
        totalVotes = 0;
    }

    override void OnUpdate (float dt)
    {
       //ScriptedWidgetEventHandler CountdownTimer -= dt;
        
        if(CountdownTimer <= 0)
        {
            ChangeState(new MapVoteEndedEvent(votes));
            return;
        } 
    }

    void RegisterVote(int index)
    {
        votes[index] = (votes[index] + 1); // If its out of range it doesn't throw a error
        totalVotes++;

        ref array<Man> m_Players = new array<Man>();
        GetGame().GetWorld().GetPlayerList(m_Players);
        if(totalVotes >= m_Players.Count() && m_Players.Count() != 0)
        {
            ChangeState(new MapVoteEndedEvent(votes));
        }
    }

    void RemoveVote(int index)
    {
        votes[index] = (votes[index] - 1);
        totalVotes--;
    }

    override int GetStateId()
    {
        return 2;
    }
}

class GGPlayingState : GunGameStateBase
{
    override void OnEntry(GunGameEventBase e)
    {
        auto ce = MapVoteEndedEvent.Cast(e);
        LocationManager<GunGameLocation>.instance.SetCurrentMapByIndex(ce.targetMap);

        ref array<Man> m_Players = new array<Man>();
        GetGame().GetWorld().GetPlayerList(m_Players);

        foreach (auto player : m_Players)
        {
            PlayerBase pb = PlayerBase.Cast(player);

            pb.GunGameRespawn();

            WeaponCreationManager.instance.SetWeapon(pb);
        }

    }

    override int GetStateId()
    {
        return 3;
    }
}