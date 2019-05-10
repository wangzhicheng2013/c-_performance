#include <iostream>
#include <memory>
using namespace std;
template <class T>
class CreatorNew
{
public:
	CreatorNew() {
		cout << "Create CreatorNew Obj!" << endl;
	}
	~CreatorNew() {
		cout << "Destroy CreatorNew Obj!" << endl;
	}
public:
	shared_ptr<T>CreateObj() {
		return shared_ptr<T>(new T);
	}
};
template <class T>
class CreatorStatic
{
public:
	CreatorStatic() {
		cout << "Create CreatorStatic Obj!" << endl;
	}
	~CreatorStatic() {
		cout << "Destroy CreatorStatic Obj!" << endl;
	}
public:
	T &CreateObj() {
		static T _t;
		return _t;
	}
};
template< template< class > class CreationPolicy >
class WidgetManager : public CreationPolicy< WidgetManager<CreationPolicy> >
{
public:
	WidgetManager() { cout << "Create WidgetManager Obj !" << endl; }

	~WidgetManager() { cout << "Destroy WidgetManager Obj !" << endl; }
};

int main() {
	cout << "------------- Create WidgetManager Object ! ------------" << endl;

	WidgetManager< CreatorNew > a_wid;

	WidgetManager< CreatorStatic > b_wid;

	cout << endl << "-- Create WidgetManager Object With CreateObj Method (New) ! --" << endl;

	a_wid.CreateObj();

	cout << endl << "-- Create WidgetManager Object With CreateObj Method (Static) ! --" << endl;

	b_wid.CreateObj();

	cout << endl << "------------ Destroy WidgetManager Object ! ------------" << endl;
	return 0;
}