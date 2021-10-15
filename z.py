def f(expo, expo2):
    vp1 = expo+expo2
    vp2 = expo-expo2
    avp1 = abs(vp1)
    avp2 = abs(vp2)
    if (avp1 <= 1 and avp2 <= 1):
        return vp1, vp2
    scaleCorrect = 1.0/max(avp1, avp2)
    return vp1*scaleCorrect, vp2*scaleCorrect
