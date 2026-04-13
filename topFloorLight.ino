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


        if (Local_topLightAuto && (Hours >= 21 || Hours <= 4))
        {
            if (!sFlag14)
            {
                pcf1.digitalWrite(topLightPin, HIGH);
                sFlag14 = true;
            }
        }
        else
        {
            if (sFlag14)
            {
                pcf1.digitalWrite(topLightPin, LOW);
                sFlag14 = false;
            }
        }
    
}