
class Room {
private:
	int id;
	bool objects;
	int corr_id;

public:
	Room();
	//getters
	int get_id();
	bool get_objects();
	int get_corr_id();
	//setters
	void set_id(int id_);
	void set_objects(bool objects_);
	void set_corr_id(int id_);
	
};
Room::Room() {}
int Room::get_id() {
	return id;
}
void Room::set_id(int id_) {
	id = id_;
}
bool Room::get_objects() {
	return objects;
}
void Room::set_objects(bool objects_) {
	objects = objects_;
}
int Room::get_corr_id(){
	return corr_id;
}
void Room::set_corr_id(int id_) {
	corr_id = id;
}