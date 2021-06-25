using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShareWeapon : MonoBehaviour
{
    public float duration;
    public float coolDown;
    public float damage;
    public BoxCollider2D attackCollider;

    protected virtual void Start() {
        // attackCollider = GetComponent<BoxCollider2D>();
        // Debug.Log("sssss");
        // UnAtack();
    }

    protected virtual void Awake() {
        attackCollider = GetComponent<BoxCollider2D>();
        UnAtack();
    }


    public void Atack() {
        attackCollider.enabled = true;
    }

    public void UnAtack() {
        attackCollider.enabled = false;
    }

    void OnTriggerEnter2D(Collider2D col) {
        if (col.gameObject.tag == "BodyPart") {
            col.gameObject.GetComponent<ShareBodyPart>().Hit(damage);
        }
    }
}
