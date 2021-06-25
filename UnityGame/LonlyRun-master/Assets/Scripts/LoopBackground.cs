using System.Collections;
using System.Collections.Generic;
using UnityEngine;


public class LoopBackground : MonoBehaviour
{
    public float speed = 0;
    public float start_x = 0;
    public float end_x = 0;

    private PlayerControl playerControl;

    // Start is called before the first frame update
    void Start()
    {
        playerControl = FindObjectOfType<PlayerControl>();
    }


    // Update is called once per frame
    void FixedUpdate()
    {
        transform.Translate(Vector2.left * (speed * playerControl.speed) * 0.02f);
        //transform.position += Vector3.left * (speed * playerControl.speed) * 0.02f;
        if (transform.position.x <= end_x) {
            transform.position = new Vector3(start_x, transform.position.y, transform.position.z);
        }
        
    }
}
