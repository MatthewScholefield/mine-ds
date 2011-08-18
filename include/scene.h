class Scene{
public:
Scene ();
~Scene ();
virtual void update();  
virtual void start();
void ChangeScene(Scene* nc);
Scene* nxtsc;
};
