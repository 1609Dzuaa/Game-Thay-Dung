#pragma once
//====================================================================//
//Class này dùng để liên kết dữ liệu giữa 2 màn chơi là 1-1 và WORLD
//Tống hết các dữ liệu static (tồn tại trong suốt quá trình chạy game)
//vào class này để dễ quản lý cũng như Refresh
//====================================================================//

#define CARD_UNDRAW_TIME	200

struct Entrance
{
	int ID;
	int isPassed;
	//Vì Kh đặt kiểu D3DXVector4 đc nên chia ra thành 4 biến kiểu Float
	float BlockDirectL, BlockDirectT, BlockDirectR, BlockDirectB;
	float x, y;
	//Chứa ID của cái Entrance đó 
	//Một biến kiểm tra xem đã Pass Entrance đó hay chưa
	//Và vị trí của Entrance đấy để set vị trí mới cho MarioWorld
};

struct Card
{
	int type;
	int aniID;
	int isAllowToRender;//Check cho phép vẽ Card lên Hud khi hiện dòng "You Got ..." cuối Game
	int draw; //2 biến này dùng để vẽ card chớp 
	int undraw; //2 biến này dùng để vẽ card chớp
	int isInitUndraw;
	int NoFlashAnymore; //Để nhận biết card này 0 chớp đc nữa
	unsigned long long int undraw_time; //Để kiểu này vì ULONGLONG nó báo lỗi
	unsigned long long int draw_time;
};

class CDataBindings
{
	static CDataBindings* __DBInstance;	//Đảm bảo chỉ có duy nhất 1 Instance
public:
	static Entrance WorldEntrance[7]; //Mảng các Entrance đã vượt qua
	static Card cardCollected[3];	//Mảng các Card lụm đc
	static int NumEntrancePass; //Số lượng Entrance đã vượt qua
	static int numCardCollected; //Số lượng Card đã lụm
	static int HP;
	static int coin;	//Ăn Coin cho 50đ, 100 coin đổi 1 mạng, coin từ 99 về 0
	static int points;
	static int TypeOfCardCollected;
	static int IsStopWatch; //ngưng mọi hoạt động khi Mario đang tiến hoá hoặc chết
	static int IsCanPlay; //Cho phép chơi (khi đã hết hiện thanh Hud Start)
	static int timer; //có nên thêm vào hay kh ?
	CDataBindings() {};
	~CDataBindings() {};
	static CDataBindings* GetInstance();

	//Dùng để set các thuộc tính cho Card Hud khi nhặt Card ở cuối Game
	void SetTypeCardAndAniID(int para, int type);

	//Dùng để handle vẽ card chớp tại Hud
	void HandleCardDrawState(Card& card_para);

	//Dùng để set thuộc tính được phép vẽ Card lên Hud
	void SetAllowToRenderCard() { cardCollected[numCardCollected].isAllowToRender = 1; }

	//Dùng để Refresh mọi dữ liệu để bắt đầu màn chơi mới
	void RefreshAllData();
};