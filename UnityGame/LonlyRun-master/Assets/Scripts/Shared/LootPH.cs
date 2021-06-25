using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
public class LootPH : MonoBehaviour
{
    public GameObject player;
    public Rigidbody2D rb;
    public float my_v;

    void Awake()
    {
        rb = GetComponent<Rigidbody2D>();
    }
    int signRange(float x, float y) {
        return y - x > 0 ? 1 : -1;
    }
    void Update()
    {
        my_v = player.GetComponent<ShareModel>().speed;
        if (rb.gravityScale == 0) {
            rb.velocity = new Vector2(-my_v + signRange(transform.position.x, player.transform.position.x)*4f
            *(float)Math.Pow(Math.Abs(transform.position.x - player.transform.position.x) + 1f, -2), 
            signRange(transform.position.y, player.transform.position.y)*5f*
            (float)Math.Pow(Math.Abs(transform.position.x - player.transform.position.x) + 2f, -2));
        }
    }

    void OnCollisionStay2D(Collision2D cl) {
        if (cl.gameObject.tag == "Platform") {
            rb.gravityScale = 0;
        }
    }
}