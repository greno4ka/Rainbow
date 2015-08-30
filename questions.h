inline double k(double x)
{
    return -2.43712*pow(10,-20)*pow(x,7)+1.12669*pow(10,-16)*pow(x,6)-2.17325*pow(10,-13)*pow(x,5)+2.27935*pow(10,-10)*pow(x,4)-1.41146*pow(10,-7)*pow(x,3)+0.0000519794*pow(x,2)-0.010683*x+2.30172;
}

double what_angle(double wave, int var)
{
    double ug,     // nessesary distance
           dir,   // direction - depends on var
           x0,y0, // point0
           x1,y1, // point1
           x2,y2; // point2 - external (for reformed)
    if (var==1) // 1 or 2 - if 1 rainbow or 2
    {
        dir=1;
        ug=0.87;
    }
    else
    {
        dir=-1;
        ug=0.96;
    }
    Line Input(0,1,-dir*ug*R,wave),Reformed, Rad; // Rad - Radius to the point1

    Input.getpoint0(&x0, &y0);
    Rad.get_koefs(x0,y0,0,0);
    Reformed=Rad;                               // we're get reformed from radius
    Reformed.snell(Input,k(Input.l()));
    Input=Reformed;

    Input.getpoint1(&x1, &y1, x0, y0);
    Rad.get_koefs(x1,y1,0,0);
    Input.reflect(Rad);
    x0=x1;
    y0=y1;

    Input.getpoint1(&x1, &y1, x0, y0);
    Rad.get_koefs(x1,y1,0,0);
    if (var==1)
    {
        Reformed=Rad;                         // we're get reformed from radius again
        Reformed.snell(Input,1/k(Input.l())); // 1/k cause goin' from inside out
        Reformed.getpoint2(&x2,&y2,x1,y1);
    }
    else
    {
        Input.reflect(Rad);
        x0=x1;
        y0=y1;

        Input.getpoint1(&x1, &y1, x0, y0);
        Rad.get_koefs(x1,y1,0,0);
        Reformed=Rad;                         // we're get reformed from radius again
        Reformed.snell(Input,1/k(Input.l())); // 1/k cause goin' from inside out
        Reformed.getpoint2(&x2,&y2,x1,y1);
    }
    return Reformed.xi();
}

double func(double wave, double angle, int var)
{
    return what_angle(wave,var)-angle;
}

double what_wave(double angle, int var)
{
    double a=480,b=780,
           eps=1;
    while(fabs(b-a)>eps)
    {
        a=b-(b-a)*func(b,angle,var)/(func(b,angle,var)-func(a,angle,var));
        b=a-(a-b)*func(a,angle,var)/(func(a,angle,var)-func(b,angle,var));
    }    // a - i-1, b - i-тый члены
    return b;
}

inline int invert_wave(int wave)
{
    return -wave+1160;
}
