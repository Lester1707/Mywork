using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static ShareWeapon;

public class DefaultWeapon : ShareWeapon
{
    protected override void Awake()
    {
        base.Awake();
        duration = 0.5f;
        coolDown = 0.5f;
        damage = 3f;

    }
}
