using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static ShareModel;

public class BirdTest : ShareModel
{
    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();
        speed = 1;
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        Move(false);
    }
    override public void Kill() {
        Debug.Log("222");
        GetComponent<BoxCollider2D>().enabled = false;
        Rigidbody.gravityScale = 3;
        Rigidbody.velocity = new Vector2(Rigidbody.velocity.x + 8, Rigidbody.velocity.y + 5);
        if (true)
            Spawn_Loot();
        head_part.Collider.enabled = false;
        Destroy(gameObject, 5);
    }

    /*
    void OnTriggerEnter2D(Collider2D col) {
        if (col.gameObject.tag == "BodyPart") {
            col.gameObject.GetComponent<ShareBodyPart>().Hit(100);
        }
    }
    */
}
