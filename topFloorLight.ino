void TopLight()
{
    if (topLight)
    {
        if (!sFlag13)
        {
            pcf1.digitalWrite(topLightPin, HIGH);
            sFlag13 = true;
        }
    }
    else
    {
        if (sFlag13)
        {
            pcf1.digitalWrite(topLightPin, LOW);
            sFlag13 = false;
        }
    }

    if (Local_topLightAuto && (Hours >= 21 || Hours <= 4 || (Hours == 5 && Min <= 30)))
    {
        if (!sFlag14)
        {
            topLight = true;
            sFlag14 = true;
        }
    }
    else
    {
        if (sFlag14)
        {
            topLight = false;
            sFlag14 = false;
        }
    }
}