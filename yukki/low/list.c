#include "lisp.h"
#include "list.h"

// set value
var_t *set_var_value(var_t *top, const char *name, int value){
	var_t *v;
	if(top != NULL){
		v = top;
		while(v != NULL){
			if(strcmp(name, v->name) == 0){
				v->value = value;
				return top;
			}
			v = v->next;
		}
	}

	// add new var_t
	v = (var_t *)malloc(sizeof(var_t));
	v->name = name;
	v->value = value;
	v->next = top;
	return v;
}

// get value
var_t *get_var_value(var_t *top, const char *name){
	if(top != NULL){
		var_t *v = top;
		while(v != NULL){
			if(strcmp(name, v->name) == 0){
				return v;
			}
			v = v->next;
		}
	}
	//printf("get_value error %s\n", name);
	return NULL;
}

// set value
func_t *set_func(func_t *top, const char *name, cons_t *args, cons_t *car){
	func_t *v;
	if(top != NULL){
		v = top;
		while(v != NULL){
			if(strcmp(name, v->name) == 0){
				v->args = args;
				v->car = car;
				return top;
			}
			v = v->next;
		}
	}

	// add new var_t
	v = (func_t *)malloc(sizeof(func_t));
	v->name = name;
	v->args = args;
	v->car = car;
	v->next = top;
	return v;
}


func_t *get_func(func_t *top, const char *name){
	if(top != NULL){
		func_t *v = top;
		while(v != NULL){
			if(strcmp(name, v->name) == 0){
				return v;
			}
			v = v->next;
		}
	}
	return NULL;
}


