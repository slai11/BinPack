from calculator import Calculator, Ward



if __name__ == '__main__':
    calc = Calculator("data45.csv", 45, isBig=True)

    werror = "Werror + "
    berror = "Berror + "
    gerror = "Gerror + "
    herror = "Herror + "
    cerror = "Cerror + "


    for i in range(45):
        werror += "B" + str(i+1) + " + "
        berror += str(calc.wardlist[i].b) + "B" + str(i+1) + " + "
        gerror += str(calc.wardlist[i].g) + "B" + str(i+1) + " + "
        herror += str(calc.wardlist[i].h) + "B" + str(i+1) + " + "
        cerror += str(calc.wardlist[i].c) + "B" + str(i+1) + " + "

    print werror[:-3] + " = "+ str(calc.wcap)
    print berror[:-3] + " = "+ str(calc.bcap)
    print gerror[:-3] + " = "+ str(calc.gcap)
    print herror[:-3] + " = " + str(calc.hcap)
    print cerror[:-3] + " = "+ str(calc.ccap)
