using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Border : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        
    }

    void OnTriggerEnter2D(Collider2D col) {
        if (col.gameObject.tag == "BodyPart") {
            col.gameObject.GetComponent<ShareBodyPart>().Hit(10000);
        }
    }
}
