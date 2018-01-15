//This class stores the current zumo position 
class ZumoPosition {
private:
  int room;
  int corridor;
public:
  ZumoPosition();
  //getters
  int get_room();
  int get_corridor();


  //setters
  void set_room(int id_);
  void set_corridor(int id_);

};
ZumoPosition::ZumoPosition() {}
int ZumoPosition::get_room() {
  return room;
}
int ZumoPosition::get_corridor() {
  return corridor;
}
void ZumoPosition::set_room(int id_) {
  room = id_;
}
void ZumoPosition::set_corridor(int id_) {
  corridor = id_;
}
