#version 110

varying vec4 v_color;

void main()
{
	// результат фрагментного шейдера Ц цвет каждого пиксел€ примитива
	// он должен быть записан во встроенную переменную gl_FragColor типа vec4
	gl_FragColor = v_color;
}