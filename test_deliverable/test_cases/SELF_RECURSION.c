int f(int a)
{

    if(a==2){
        return 12;
    }
    else{
        a=a-1;
        f(a);
    }
}
