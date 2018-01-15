// 0 refers to left  , 1 = right
class Corridor {
private:
  int corr_id;
  bool sub;
  int previous_direction;
  int current_direction;
public:
  Corridor();
  int get_corr_id();
  void set_corr_id(int id);
  bool is_sub();
  void set_sub(bool sub_);
  void set_previous_direction(int direction);
  void set_current_direction(int direction);
  int get_current_direction();
  int get_previous_direction();
};
Corridor::Corridor() {}
int Corridor::get_corr_id() {
  return corr_id;
}
void Corridor::set_corr_id(int id) {
  corr_id = id;
}
bool Corridor::is_sub() {
  return sub;
}
void Corridor::set_sub(bool sub_) {
  sub = sub;
}
void Corridor::set_current_direction(int direction) {
  current_direction = direction;
}
void Corridor::set_previous_direction(int direction) {
  previous_direction = direction;
}

int Corridor::get_current_direction() {
  return current_direction;
}
int Corridor::get_previous_direction() {
  return previous_direction;
}

