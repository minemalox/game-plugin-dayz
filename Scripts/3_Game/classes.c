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
    private string id;
    private string icon;
    private string className;
    private Object _reference;
    void _Event(string className, string icon, Object _reference) {this.className = className; this.icon = icon; this._reference = _reference; this.id = EntityAI.Cast(this._reference).GetNetworkIDString(); }
    string GetID() { return this.id; }
    string Class() { return this.className; }
    string Icon() { return this.icon; }
    Object Ref() { return this._reference; }
    bool Equals(_Event other) {
        if(this._reference == NULL) return false;
        return this._reference.GetPosition() == other._reference.GetPosition();
    }
};