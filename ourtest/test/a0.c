int f(){
    int x = 0;
    int y = 0;
    int i = 0;

    while (x<3) {
        while (i<3) {
            y++;
            i++;
        }
        x++;
        i = 0;
    }
    return y;
}
