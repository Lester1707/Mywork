using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShareBodyPart : MonoBehaviour
{
    public float HP;
    public BoxCollider2D Collider;
    public float chance_out;
    public bool live;

    void Awake () {
        live = true;
        Collider = GetComponent<BoxCollider2D>();
    }

    public void Hit(float Damage) {
            HP -= Damage;
            if (HP < 0) {
                live = false;
                Collider.enabled = false;
                transform.parent.gameObject.GetComponent<ShareModel>().Kill();
            }
    }
}
