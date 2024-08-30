#include "include.h"

class $modify(DeltaruneDialogLayer, DialogLayer) {
	struct Fields {
		DialogObject* p0;
	};
	bool init(DialogObject*, CCArray*, int);
	void displayDialogObject(DialogObject*);
};