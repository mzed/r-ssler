#include "ext.h"
#include "ext_obex.h"

typedef struct _rossler
{
	t_object r_ob;
	double r_x;
	double r_y;
	double r_z;
	double r_h;
	double r_abc[3];
	
	void *r_out0;
	void *r_out1;
	void *r_out2;
					
} t_rossler;

void rossler_bang(t_rossler *x);
void rossler_float(t_rossler *x, float n);   
void rossler_ft1(t_rossler *x, float o);
void rossler_ft2(t_rossler *x, float p);
void rossler_ft3(t_rossler *x, float q);
void rossler_assist(t_rossler *x, void *b, long m, long a, char *s);
void rossler_inletinfo(t_rossler *x, void *b, long index, char *t);
void *rossler_new(t_symbol *s, long argc, t_atom *argv);

static t_class *rossler_class;

void ext_main(void *r)
{
    t_class *c;
    c = class_new("rössler", (method)rossler_new, (method)0L, sizeof(t_rossler), 0L, A_GIMME, 0);
	
	class_addmethod(c, (method)rossler_bang, "bang", 0);
	class_addmethod(c, (method)rossler_float, "float", A_FLOAT, 0);
    class_addmethod(c, (method)rossler_ft1, "ft1", A_FLOAT, 0);
	class_addmethod(c, (method)rossler_ft2, "ft2", A_FLOAT, 0);
    class_addmethod(c, (method)rossler_ft3, "ft3", A_FLOAT, 0);
    class_addmethod(c, (method)rossler_assist, "assist", A_CANT, 0);
    class_addmethod(c, (method)rossler_inletinfo, "inletinfo", A_CANT, 0);
    
    CLASS_ATTR_DOUBLE(c, "a", 0, t_rossler, r_abc[0]);
    CLASS_ATTR_STYLE_LABEL(c, "a", 0, 0, "a");
    CLASS_ATTR_CATEGORY(c, "a", 0, "rossler");
    
    CLASS_ATTR_DOUBLE(c, "b", 0, t_rossler, r_abc[1]);
    CLASS_ATTR_STYLE_LABEL(c, "b", 0, 0, "b");
    CLASS_ATTR_CATEGORY(c, "b", 0, "rossler");
    
    CLASS_ATTR_DOUBLE(c, "c", 0, t_rossler, r_abc[2]);
    CLASS_ATTR_STYLE_LABEL(c, "c", 0, 0, "c");
    CLASS_ATTR_CATEGORY(c, "c", 0, "rossler");
    
    CLASS_ATTR_DOUBLE(c, "h", 0, t_rossler, r_h);
    CLASS_ATTR_STYLE_LABEL(c, "h", 0, 0, "h");
    CLASS_ATTR_CATEGORY(c, "h", 0, "rossler");
    
	post("rossler object by Michael F. Zbyszyński, v2.0 ©2002-2016");
	
    class_register(CLASS_BOX, c);
    rossler_class = c;
}

void rossler_bang(t_rossler *x)
{
	double xnew;
	double ynew;
	double znew;
	
	xnew = x->r_x + x->r_h * (- x->r_y - x->r_z);
	ynew = x->r_y + x->r_h * (x->r_x + x->r_abc[0] * x->r_y);
	znew = x->r_z + x->r_h * (x->r_abc[1] + x->r_z * (x->r_x - x->r_abc[2]));
	
	outlet_float(x->r_out2, znew);
	outlet_float(x->r_out1, ynew);
	outlet_float(x->r_out0, xnew);
	
	x->r_x = xnew;
	x->r_y = ynew;
	x->r_z = znew;
}

void rossler_float(t_rossler *x, float n)
{
	x->r_x = n;
    rossler_bang(x);
}

void rossler_ft1(t_rossler *x, float n)
{
	x->r_y = n;
}

void rossler_ft2(t_rossler *x, float n)
{
	x->r_z = n;
}

void rossler_ft3(t_rossler *x, float n)
{
	x->r_h = n;
}

void rossler_assist(t_rossler *x, void *b, long m, long a, char *s)
{
    if (m == ASSIST_OUTLET)
        switch (a) {
            case 0:
                sprintf(s,"X result of rossler attractor");
                break;
            case 1:
                sprintf(s,"Y result of rossler attractor");
                break;
            case 2:
                sprintf(s,"Z result of rossler attractor");
                break;
        }
    else {
        switch (a) {
            case 0:
                sprintf(s,"float X or bang");
                break;
            case 1:
                sprintf(s,"float Y");
                break;
            case 2:
                sprintf(s,"float Z");
                break;
            case 3:
                sprintf(s,"float H");
                break;
        }
    }
}

void rossler_inletinfo(t_rossler *x, void *b, long index, char *t)
{
    if (index > 0) {
        *t = 1;
    }
}

void *rossler_new(t_symbol *s, long argc, t_atom *argv)
{
	t_rossler *x;
    x = (t_rossler *)object_alloc(rossler_class);
    
    x->r_abc[0] = x->r_abc[1] = 0.2;
    x->r_abc[2] = 5.7;
    x->r_h = 0.08;
    if (argc) {
        for (int i = 0; i < argc; ++i) {
            switch (i) {
                case 0:
                case 1:
                case 2:
                    if ((argv+i)->a_type == A_FLOAT) {
                        x->r_abc[i] = (double)atom_getfloat(argv+i);
                    } else if ((argv+i)->a_type == A_LONG) {
                        x->r_abc[i] = (double)atom_getlong(argv+i);
                    }
                    break;
                case 3:
                    if ((argv+i)->a_type == A_FLOAT) {
                        x->r_h = (double)atom_getfloat(argv+i);
                    } else if ((argv+i)->a_type == A_LONG) {
                        x->r_h = (double)atom_getlong(argv+i);
                    }
                    break;
            }
        }
    }
    
    x->r_x = 0.01;
    x->r_y = 0.01;
    x->r_z = 0.01;
	
	floatin(x,3);
	floatin(x,2);
	floatin(x,1);
	
	x->r_out2 = floatout(x);
	x->r_out1 = floatout(x);
	x->r_out0 = floatout(x);

	return (x);
}