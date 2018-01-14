class Corridor {
private:
	int corr_id;
	bool sub;
	String last_corr_turn;
	String direction;
public:
	Corridor();
	int get_corr_id();
	void set_corr_id(int id);
	bool is_sub();
	void set_sub(bool sub_);
	void last_direction(String lastturn);
	void set_direction(String direction);
};

Corridor::Corridor() {}
int Corridor::get_corr_id() {
	return corr_id;
}
void Corridor::set_corr_id(int id) {
	corr_id = id;
}
bool Corridor::is_sub() {
	return is_sub;
}
void Corridor::set_sub(bool sub_) {
	sub = sub;
}
