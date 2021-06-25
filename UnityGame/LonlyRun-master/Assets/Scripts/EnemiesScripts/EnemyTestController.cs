using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static ShareModel;

public class EnemyTestController : ShareModel
{
    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();
        // LoadBodyPart("DefaultHead");
        // LoadBodyPart("DefaultBody");
        // LoadW("WeaponCloseType1");
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        Move(UnityEngine.Random.Range(0, 100) == 50);
    }
}
