#pragma once

#include "shape.h"

class CLine : public CShape
{
private:
	int x0, y0, x1, y1;

public:
	CLine(float r, float g, float b) : CShape(r, g, b)
	{

	}

	~CLine()
	{
		cout << "Se destruyo una linea" << endl;
	}

	void set(int x0, int y0, int x1, int y1)
	{
		this->x0 = x0;
		this->y0 = y0;
		this->x1 = x1;
		this->y1 = y1;
	}

	void render()
	{
		// despliegas la línea con el algoritmo de bresenham
		setColor(color[0], color[1], color[2]);

		// user putpixel de aquí en adelante... con Bresenham
		glBegin(GL_LINES);
		glVertex2i(x0, y0);
		glVertex2i(x1, y1);
		glEnd();
	}

	bool onClick(int x, int y) 
	{
		// determinar la distancia del click a la línea
		// si es mejor a un umbral (e.g. 3 píxeles) entonces
		// retornas true
		return false;
	}

	void onMove(int x, int y)
	{
	}

};
