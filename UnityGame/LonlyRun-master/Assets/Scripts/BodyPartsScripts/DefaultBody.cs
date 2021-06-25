using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static ShareBodyPart;

public class DefaultBody : ShareBodyPart
{
    // Start is called before the first frame update
    void Start()
    {
        HP = 1;
        chance_out = 1f;
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
