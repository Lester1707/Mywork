using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;
using static ShareModel;



public class PlayerControl : ShareModel
{
    public enum State {
        FREE, IDLE, DASHRETREAT,
        JUMP, RETREAT,
        ROLL, DASHBACK
    }

    public enum TouchControl {
        NON, UP, DOWN, RIGHT, LEFT, TOUCH 
        
    }

    public float slowingSpeed = 8;
    private float DashSpeed = 30;
    public float DashMultiplier = 3;
    public float DashDuration = 1;
    public float backDashSpeed = 3;
    public float RetreatSpeed = 4;
    public float DashRetriatSpeed = 1;
    public float RollDuration = 1;
    private bool inDash = false;
    private bool inRoll = false;
    public float BackDashX = -8.8f;
    private float startSpeed;
    
    public State state = State.IDLE;
    public TouchControl direction = TouchControl.NON;
    private Vector3 targetPos;
    private Vector3 startPos;
    
    public GameObject canvas;

    void OnCollisionEnter2D(Collision2D cl) {
        if (cl.gameObject.tag == "Loot") {
            //get sum loot;
            Destroy(cl.gameObject);
        }
    }


    // Start is called before the first frame update
    protected override void Start()
    {
        base.Start();
        startPos = transform.position;
        startSpeed = speed;
        DashSpeed = speed * DashMultiplier;
        isAttaking = false;
        LoadW("WeaponCloseType1");
        //LoadBodyPart("DefaultHead");
        //LoadBodyPart("DefaultBody");
    }

    // Update is called once per frame
    private void FixedUpdate() {
        startSpeed += 0.001f;
    }
    
    void Update()
    {
        SwipeMouse();
        //Swipe();
        // адоптирование скорости после ее изменения
        if (!live) {
            speed = speed - (speed) * slowingSpeed * Time.deltaTime / 3;
            return;
        }
        speed = speed - (speed - startSpeed) * slowingSpeed * Time.deltaTime / 3;
        //
        switch (state) {
            case State.IDLE:
                if ((Input.GetKeyDown(KeyCode.UpArrow) || direction == TouchControl.UP) && TouchsGround() && !isAttaking && !inRoll)
                {
                    Jump();
                    state = State.JUMP;
                } else if ((Input.GetKeyDown(KeyCode.RightArrow)|| direction == TouchControl.RIGHT) && !inDash && !inRoll) 
                {
                    StartCoroutine("Dash");
                } else if ((Input.GetKeyDown(KeyCode.LeftArrow)|| direction == TouchControl.LEFT) && !isAttaking && !inRoll) 
                {
                    state = State.DASHBACK;
                } else if ((Input.GetKeyDown(KeyCode.DownArrow)|| direction == TouchControl.DOWN) && !isAttaking && !inRoll) 
                {
                    StartCoroutine("Roll");
                } else if ((Input.GetKeyDown(KeyCode.Space) || direction == TouchControl.TOUCH) && !isAttaking && !inRoll ) 
                {
                    Attak();
                }
                break;

            case State.JUMP:
                if(TouchsGround()) {
                    state = State.RETREAT;
                } else if ((Input.GetKeyDown(KeyCode.RightArrow) || direction == TouchControl.RIGHT) && !inDash) 
                {
                    StartCoroutine("Dash");
                } else if ((Input.GetKeyDown(KeyCode.DownArrow)|| direction == TouchControl.DOWN) && !isAttaking) 
                {
                    StartCoroutine("Roll");
                } else if ((Input.GetKeyDown(KeyCode.Space) || direction == TouchControl.TOUCH) && !isAttaking ) 
                {
                    StartCoroutine("Attak");
                }
                break;

            case State.RETREAT:

                if ((Input.GetKeyDown(KeyCode.UpArrow) || direction == TouchControl.UP) && !isAttaking && !inRoll)
                {
                    Rigidbody.velocity = new Vector2(Rigidbody.velocity.x, JumpSpeed);
                    state = State.JUMP;
                } else if ((Input.GetKeyDown(KeyCode.DownArrow) || direction == TouchControl.DOWN) && !isAttaking  && !inRoll) 
                {
                    StartCoroutine("Roll");
                } if ((Input.GetKeyDown(KeyCode.Space) || direction == TouchControl.TOUCH) && !isAttaking  && !inRoll) 
                {
                    Attak();
                }
                //From back
                targetPos = new Vector3(startPos.x + 1, startPos.y, startPos.z);
                if ((Input.GetKeyDown(KeyCode.RightArrow) || direction == TouchControl.RIGHT)  && !inRoll) 
                {
                    state = State.DASHRETREAT;
                }
                transform.position = Vector3.Lerp(transform.position, targetPos, RetreatSpeed * Time.deltaTime);
                if ((startPos.x - transform.position.x) <= 0.1) {
                    state = State.IDLE;
                }
                
                break;

            case State.DASHRETREAT:
                targetPos = new Vector3(startPos.x + 1, startPos.y, startPos.z);
                transform.position = Vector3.Lerp(transform.position, targetPos, DashRetriatSpeed * Time.deltaTime);
                if ((startPos.x - transform.position.x) <= 0.1) {
                    state = State.IDLE;
                }
                break;

            case State.DASHBACK:
                targetPos = new Vector3(BackDashX, transform.position.y, transform.position.z);
                Rigidbody.velocity = new Vector2(0, 0);
                transform.position = Vector3.Lerp(transform.position, targetPos, backDashSpeed * Time.deltaTime);
                if (Mathf.Abs(transform.position.x - targetPos.x) <= 1) {
                    state = State.RETREAT;
                }
                break;
        } 

        
    }

    IEnumerator Roll() 
    {
        inRoll = true;
        animator.SetBool("isRoll", true);
        Rigidbody.velocity = new Vector2(0, -20);
        state = State.RETREAT;
        yield return new WaitForSeconds(RollDuration);
        animator.SetBool("isRoll", false);
        inRoll = false;
    }
    IEnumerator Dash() 
    {
        speed = DashSpeed;
        inDash = true;
        yield return new WaitForSeconds(DashDuration);
        inDash = false;
    }

    bool TouchsGround() {
        if (transform.position.y <= -2.18 && Mathf.Abs(Rigidbody.velocity.y) < 0.01) {
            return true;
        }
        return false;
    }

    //swipe controlls 
    Vector2 firstPressPos;
    Vector2 secondPressPos;
    Vector2 currentSwipe;
 
    public void Swipe()
    {
        if(Input.touches.Length > 0)
        {
            Touch t = Input.GetTouch(0);
            if(t.phase == TouchPhase.Began)
            {
                //save began touch 2d point
                firstPressPos = new Vector2(t.position.x,t.position.y);
            }
            if(t.phase == TouchPhase.Ended)
            {
                //save ended touch 2d point
                secondPressPos = new Vector2(t.position.x,t.position.y);
                            
                //create vector from the two points
                currentSwipe = new Vector3(secondPressPos.x - firstPressPos.x, secondPressPos.y - firstPressPos.y);
                
                //normalize the 2d vector
                currentSwipe.Normalize();
    
                //swipe upwards
                if(currentSwipe.y > 0 && currentSwipe.x > -0.5f && currentSwipe.x < 0.5f)
                {
                    Debug.Log("up swipe");
                    direction = TouchControl.UP;
                }
                //swipe down
                else if(currentSwipe.y < 0 && currentSwipe.x > -0.5f && currentSwipe.x < 0.5f)
                {
                    Debug.Log("down swipe");
                    direction = TouchControl.DOWN;
                }
                //swipe left
                else if(currentSwipe.x < 0 && currentSwipe.y > -0.5f && currentSwipe.y < 0.5f)
                {
                    Debug.Log("left swipe");
                    direction = TouchControl.LEFT;
                }
                //swipe right
                else if(currentSwipe.x > 0 && currentSwipe.y > -0.5f && currentSwipe.y < 0.5f)
                {
                    Debug.Log("right swipe");
                    direction = TouchControl.RIGHT;
                }
                else {
                    direction = TouchControl.TOUCH;
                }
            }
        } else {
            direction = TouchControl.NON;
        }
    }

    
    public void SwipeMouse()
    {
        if(Input.GetMouseButtonDown(0))
        {
            //save began touch 2d point
            firstPressPos = new Vector2(Input.mousePosition.x,Input.mousePosition.y);
        } 
        else if(Input.GetMouseButtonUp(0))
        {
                //save ended touch 2d point
            secondPressPos = new Vector2(Input.mousePosition.x,Input.mousePosition.y);
        
                //create vector from the two points
            currentSwipe = new Vector2(secondPressPos.x - firstPressPos.x, secondPressPos.y - firstPressPos.y);
            
            //normalize the 2d vector
            currentSwipe.Normalize();
    
            //swipe upwards
            if(currentSwipe.y > 0 && currentSwipe.x > -0.5f && currentSwipe.x < 0.5f)
            {
                Debug.Log("up swipe");
                direction = TouchControl.UP;
            }
            //swipe down
            else if(currentSwipe.y < 0 && currentSwipe.x > -0.5f && currentSwipe.x < 0.5f)
            {
                Debug.Log("down swipe");
                direction = TouchControl.DOWN;
            }
            //swipe left
            else if(currentSwipe.x < 0 && currentSwipe.y > -0.5f && currentSwipe.y < 0.5f)
            {
                Debug.Log("left swipe");
                direction = TouchControl.LEFT;
            }
            //swipe right
            else if(currentSwipe.x > 0 && currentSwipe.y > -0.5f && currentSwipe.y < 0.5f)
            {
                Debug.Log("right swipe");
                direction = TouchControl.RIGHT;
            }
            else {
                direction = TouchControl.TOUCH;
            }
        } else {
            direction = TouchControl.NON;
        }
    }

    override public void Kill() {
        
        canvas.GetComponent<Canvas>().enabled = true;
        live = false;
        animator.SetBool("live", live);
        weapon.UnAtack();
    }
}
