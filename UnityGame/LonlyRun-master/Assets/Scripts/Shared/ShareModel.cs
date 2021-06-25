using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using static ShareWeapon;
using static LootPH;

public class ShareModel : MonoBehaviour
{
    public float speed = 0;
    protected Rigidbody2D Rigidbody;
    public Animator animator;
    private PlayerControl playerControl;

    public bool live;
    protected bool AttakCD = false;
    protected bool isAttaking = false;

    public float JumpSpeed = 40;
    public float PreAttakDuration = 0.2f;
    public float AttakDuration = 0.5f;
    public float AttakCDDuration = 0.5f;
    
    public ShareWeapon weapon;
    public ShareBodyPart body_part;
    public ShareBodyPart head_part;

    protected virtual void Start() {
        live = true;
        //weapon = gameObject.GetComponent(typeof(ShareWeapon)) as ShareWeapon;
        playerControl = FindObjectOfType<PlayerControl>();
        Rigidbody = GetComponent<Rigidbody2D>();
    }

    public void Attak() {
        StartCoroutine("AttakRut");
    }

    IEnumerator AttakRut() 
    {
        if (AttakCD) {
            yield break;
        }

        isAttaking = true;
        AttakCD = true;
        animator.SetBool("isAttaking", isAttaking);
        yield return new WaitForSeconds(PreAttakDuration);
        if (live)
            weapon.Atack();
        yield return new WaitForSeconds(AttakDuration);
        weapon.UnAtack();
        isAttaking = false;
        animator.SetBool("isAttaking", isAttaking);
        yield return new WaitForSeconds(AttakCDDuration);
        AttakCD = false;
        
    }

    public bool Jump() {
        Debug.Log("Jump");
        Rigidbody.velocity = new Vector2(Rigidbody.velocity.x, JumpSpeed);
        return true;
    }

    public void Move(bool conditional) {
        transform.Translate(Vector2.left * (speed + playerControl.speed) * 0.02f);
        if (conditional && weapon)
            Attak();
        // if (transform.position.x < death_point.transform.position.x) {
        //     return false;
        // }
    }

    public void Spawn_Loot() {
        LootPH obj = Instantiate(Resources.Load("Loot", typeof(LootPH))) as LootPH;
        obj.transform.position = new Vector3(transform.position.x, transform.position.y, transform.position.z);
        obj.player = GameObject.Find("Player");
        obj.rb.AddForce(new Vector3(Random.Range(2f, 4f), Random.Range(-1f, 1f), 0), ForceMode2D.Impulse);
    }

    virtual public void Kill() {
        //Debug.Log("222");
        live = false;
        animator.SetBool("live", live);
        //GetComponent<BoxCollider2D>().enabled = false;
        if (weapon != null)
            weapon.UnAtack();
        if (true)
            Spawn_Loot();
        if (body_part != null)
            body_part.Collider.enabled = false;
        if (head_part != null)
            head_part.Collider.enabled = false;
        Destroy(weapon);
        Destroy(gameObject, 5);

    }

    public void LoadW(string name) {
        weapon = Instantiate(Resources.Load("Weapons/" + name, typeof(ShareWeapon))) as ShareWeapon;
        weapon.transform.parent = transform;
        if (weapon.tag == "MilliWeapon") {
            if (tag == "Player")
                weapon.transform.localPosition = new Vector3((GetComponent<BoxCollider2D>().size.x +
                    weapon.attackCollider.size.x * weapon.transform.localScale.x)/2, 0, 0);
            if (tag == "Enemy")
                weapon.transform.localPosition = new Vector3(-(GetComponent<BoxCollider2D>().size.x +
                    weapon.attackCollider.size.x * weapon.transform.localScale.x)/2, 0, 0);
        }
        // if (weapon.attackField.tag == "ArmorHelmet")
        //     ShareWeapon.weapon.transform.localPosition = new Vector3(0, GetComponent<SpriteRenderer>().size.y/2 -
        //         GetComponent<CircleCollider2D>().radius, 0);
    }
    public void LoadBodyPart(string type, string name) {
        if (type == "Body") {
            Vector3 tr = body_part.transform.position;
            Vector2 sz = body_part.Collider.size;
            Vector2 of = body_part.Collider.offset;
            body_part = Instantiate(Resources.Load("BodyParts/" + name, typeof(ShareBodyPart))) as ShareBodyPart;
            body_part.transform.position = tr;
            body_part.Collider.size = sz;
            body_part.Collider.offset = of;
        }
        if (type == "Head") {
            Vector3 tr = head_part.transform.position;
            Vector2 sz = head_part.Collider.size;
            Vector2 of = head_part.Collider.offset;
            head_part = Instantiate(Resources.Load("BodyParts/" + name, typeof(ShareBodyPart))) as ShareBodyPart;
            head_part.transform.position = tr;
            head_part.Collider.size = sz;
            head_part.Collider.offset = of;
            
        }
        // BodyParts.Add(name, Instantiate(Resources.Load("BodyParts/" + name, typeof(ShareBodyPart))) as ShareBodyPart);
        // BodyParts[name].transform.SetParent(transform, false);
        
    }

    void OnTriggerEnter2D(Collider2D cl) {
        if (cl.gameObject.tag == "Player" && live) {
            animator.SetTrigger("Stab");
            cl.gameObject.GetComponent<ShareModel>().Kill();
        }
    }
}
