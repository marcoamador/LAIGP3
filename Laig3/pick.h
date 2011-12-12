class point3Df
{	
public: 
	float x, y, z;
public:
	point3Df(void)
	{	x = 0.0;
		y = 0.0;
		z = 0.0;
	}
	point3Df(float a, float b, float c)
	{	x = a;
		y = b;
		z = c;
	}
};

class picking
{	
private:
	int nobj;
	bool *estado;
	point3Df *dAc;
	int objectoSeleccionado;
public:
	picking(int n)
	{	int i;
    
		nobj = n;
		estado = (bool *) new bool[n];
		dAc = (point3Df *)new point3Df[n];
		for (i=0; i<n; i++)
		{	estado[i] = false;
		}
		objectoSeleccionado = 0;
	}
	~picking(void)
	{	if (estado != (bool *)NULL)
			delete []estado;
		if (dAc != (point3Df *)NULL)
			delete []dAc;
	}

	void setEstado(int i)
	{	estado[i]=true;
		objectoSeleccionado = i;
	}
	bool getEstado(int i)
	{	return (estado[i]);
	}
	void resetEstados(void)
	{	int i;
		for (i=0; i<nobj; i++)
			estado[i] = false;
	}
	float getDx(int i)
	{	return(dAc[i].x);
	}
	float getDz(int i)
	{	return(dAc[i].z);
	}
	float getDy(int i)
	{	return(dAc[i].y);
	}
	void setDxyz(float x, float y, float z)
	{	dAc[objectoSeleccionado].x = x;
		dAc[objectoSeleccionado].y = y;
		dAc[objectoSeleccionado].z = z;
	}
};