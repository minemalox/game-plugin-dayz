class _AI {
    private Object _reference;
    private vector _lastPos;
    private bool _infected = false;
    void _AI(Object _reference, bool infected) { this._reference = _reference; this._infected = infected; this._lastPos = _reference.GetPosition(); };
    Object Ref() { return this._reference; }
    bool IsInfected() { return this._infected; }
    bool IsActive() {
        if(!this.IsInfected()) return false;
        // This is a bit janky, but we have no other option
        vector currentPos = this._reference.GetPosition();
        if(currentPos != this._lastPos) {
            this._lastPos = currentPos;
            return true;
        } else {
            return false;
        }
    }
};

class _Vehicle {
    private string id;
    private string className;
    private vector _lastPos;
    private Object _reference;
    void _Vehicle(Object _reference) { this._reference = _reference; this.className = this.Ref().GetType(); this.id = EntityAI.Cast(this._reference).GetNetworkIDString(); };
    string GetID() { return this.id; }
    string Class() { return className; }
    Object Ref() { return this._reference; }
    bool HasUpdated() {
        if(this._reference == NULL) return true;
        vector currentPos = this._reference.GetPosition();
        if(currentPos != this._lastPos) {
            this._lastPos = currentPos;
            return true;
        } else {
            return false;
        }
    }
};

class _Event {
    private int id_low;
    private int id_high;
    private string id;
    private string icon;
    private string className;
    //! Keeping object reference to class House (aka, wrecks from events) has weird side-effects for some reason,
    //! it will break initialization of other, unrelated classes (EnfScript bug?).
    //! Use network ID to get object dynamically when needed instead.
    void _Event(string className, string icon, Object _reference) {
        this.className = className;
        this.icon = icon;
        _reference.GetNetworkID(id_low, id_high);
        this.id = id_high.ToString() + id_low.ToString();
    }
    string GetID() { return this.id; }
    string Class() { return this.className; }
    string Icon() { return this.icon; }
    Object Ref() { return GetGame().GetObjectByNetworkId(this.id_low, this.id_high); }
    bool Equals(_Event other) {
        Object _reference = Ref();
        Object _other_reference = other.Ref();
        if(!_reference || !_other_reference) return false;
        return _reference.GetPosition() == _other_reference.GetPosition();
    }
};