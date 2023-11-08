// RFIDDlg.cpp: 구현 파일
//
#include "is_d2xx.h"
#include "pch.h"
#include "framework.h"
#include "RFID.h"
#include "RFIDDlg.h"
#include "afxdialogex.h"
#include <iostream>

// sound 출력용
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include "mmsystem.h"
#include <mysql.h>
#include <string>
//#include <atlstr.h>
#pragma comment(lib,"winmm.lib") // 라이브러리 불러오기

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
//	afx_msg LRESULT OnMymsg(WPARAM wParam, LPARAM lParam);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CRFIDDlg 대화 상자
CRFIDDlg::CRFIDDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RFID_DIALOG, pParent)
	, m_strNamE(_T(""))
	, m_strBeego(_T(""))
	, m_strWitch(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
CRFIDDlg::~CRFIDDlg()
{
	OnDisconnect();
}

void CRFIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PIC, m_picture_control);
	DDX_Text(pDX, IDC_EDIT3, m_strNamE);
	DDX_Text(pDX, IDC_EDIT2, m_strBeego);
	DDX_Text(pDX, IDC_EDIT1, m_strWitch);
}

BEGIN_MESSAGE_MAP(CRFIDDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CRFIDDlg::OnConnect)
	ON_BN_CLICKED(IDC_BUTTON3, &CRFIDDlg::OnReadOnce)
	ON_BN_CLICKED(IDC_BUTTON4, &CRFIDDlg::OnReadContinue)
	ON_BN_CLICKED(IDC_BUTTON2, &CRFIDDlg::OnDisconnect)
	ON_MESSAGE(MYMSG, &CRFIDDlg::OnMymsg)
END_MESSAGE_MAP()

// CRFIDDlg 메시지 처리기
BOOL CRFIDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CRFIDDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CRFIDDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CRFIDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRFIDDlg::OnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		//열린 포트번호 찾기
	if (is_GetDeviceNumber(&usbnumber) == IS_OK)
	{
		printf("FTDI USB To Serial 연결된 개수 : %d\n", usbnumber);
		if (is_GetSerialNumber(0, readSerialNumber) == IS_OK)
		{
			if (memcmp(changeSerialNumber, readSerialNumber, sizeof(changeSerialNumber) != 0))
			{
				if (is_SetSerialNumber(0, changeSerialNumber) == IS_OK)
				{
					printf(" USB To Serial Number 를 변경 하였습니다.\n");
					printf(" FTDI SerialNumber :  %s \n", changeSerialNumber);
				}
			}
			else
				printf(" FTDI SerialNumber :  %s \n", changeSerialNumber);
		}
	}

	//열린 포트번호로 연결
	unsigned long portNumber;
	if (is_GetCOMPort_NoConnect(0, &portNumber) == IS_OK)
	{
		printf("COM Port : %d\n", portNumber);
	}
	if (is_OpenSerialNumber(&ftHandle, readSerialNumber, 115200) != IS_OK)
	{
		printf("USB To Serial과 통신 연결 실패\n");
		//return -1;
	}
	else {
		printf("Serial포트 %d와 통신 연결성공!! \n", portNumber);
	}
	Sleep(100);
	flag_r = 0;
}

CString cname = _T(""), cbeego = _T(""), cimage = _T(""), cwitch = _T("");  // DB에서 불러올 정보를 담을 그릇 초기화
CString temp, temp1 = _T("");  // 카드 UID를 담을 그릇 초기화
CString temp5 = _T("81 93 51 20 ");  // 해당 카드 번호를 각각 적음
CString temp6 = _T("f1 0c 9d af ");
CString temp7 = _T("2a d7 68 cb 50 01 04 e0 ");
CString temp8 = _T("3e fe 68 cb 50 01 04 e0 ");

void CRFIDDlg::OnReadOnce()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 이미지 출력 부분
	CRect rect;  // 픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_picture_control.GetWindowRect(rect);  // GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc;  // 픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_picture_control.GetDC();  // 픽쳐 컨트롤의 DC를 얻는다.
	CImage image;  // 불러오고 싶은 이미지를 로드할 CImage

	if (flag_r == 0)
	{
		// table명 : member, book, wine, cow, song 등 수정만 하면 모두 불러올 수 있게 설계
		if (is_WriteReadCommand(ftHandle, CM1_ISO14443AB, CM2_ISO14443A_ACTIVE + BUZZER_ON,  //ISO14443A모드로 읽기
			writeLength, wirteData, &readLength, readData) == IS_OK ||  // or로 모두 읽을 수 있도록 함
			is_WriteReadCommand(ftHandle, CM1_ISO15693, CM2_ISO15693_ACTIVE + BUZZER_ON,  //ISO15693모드로 읽기
				writeLength, wirteData, &readLength, readData) == IS_OK)
		{
			// DB 부분
			unsigned int timeout_sec = 1;
			char query[1024];
			MYSQL* conn;
			conn = mysql_init(NULL);
			mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout_sec);
			MYSQL_ROW row;
			MYSQL_RES* result;
			MYSQL* conn_result;
			conn_result = mysql_real_connect(conn, "127.0.0.1", "root", "qkqh123*", "mysql", 3306, NULL, 0);

			for (int i = 0; i < readLength; i++)  // 카드 정보를 temp1이라는 변수에 저장
			{
				temp.Format(_T("%02x "), readData[i]);
				temp1 += temp;
			}
			printf("\n");

			if (NULL == conn_result)
			{
				std::cout << "DB Connection Fail" << std::endl;
			}
			else
			{
				std::cout << "DB Connection Success" << std::endl;

				if (!(temp1.Compare(temp5)))  // temp1에 담긴 카드 정보와 사전에 저장했던 번호를 비교하여 같으면 0을 호출
				{
					sprintf_s(query, 1024, "SELECT * FROM book WHERE num = 1");  //14443 - 81935120

					if (mysql_query(conn, query))  // Send Query
					{
						std::cout << "SELECT Fail" << std::endl;
					}

					result = mysql_store_result(conn);
					int fields = mysql_num_fields(result);    // 필드 갯수 구함  

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];  // DB에서 불러온 정보를 각각의 그릇에 담는 역할
						cbeego = row[2];
						cimage = row[3];
						cwitch = row[4];

						image.Load(cimage);  // 사진 출력 부분
						m_strWitch = cwitch;  // 위치 출력 부분
						m_strBeego = cbeego;  // 비고 출력 부분
						m_strNamE = cname;  // 이름 출력 부분
						image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);  //이미지를 픽쳐 컨트롤 크기로 조정
						ReleaseDC(dc);//DC 해제
						UpdateData(FALSE);  // 변화된 정보를 한번에 출력되도록 하기
					}
					mysql_free_result(result);
					mysql_close(conn);
				}

				if (!(temp1.Compare(temp6)))  // temp1에 담긴 카드 정보와 사전에 저장했던 번호를 비교하여 같으면 0을 호출
				{
					sprintf_s(query, 1024, "SELECT * FROM book WHERE num = 2");  //14443 - f10c9daf

					if (mysql_query(conn, query))  // Send Query
					{
						std::cout << "SELECT Fail" << std::endl;
					}

					result = mysql_store_result(conn);
					int fields = mysql_num_fields(result);    // 필드 갯수 구함  

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];  // DB에서 불러온 정보를 각각의 그릇에 담는 역할
						cbeego = row[2];
						cimage = row[3];
						cwitch = row[4];

						image.Load(cimage);  // 사진 출력 부분
						m_strWitch = cwitch;  // 위치 출력 부분
						m_strBeego = cbeego;  // 비고 출력 부분
						m_strNamE = cname;  // 이름 출력 부분
						image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);  //이미지를 픽쳐 컨트롤 크기로 조정
						ReleaseDC(dc);//DC 해제
						UpdateData(FALSE);  // 변화된 정보를 한번에 출력되도록 하기
					}
					mysql_free_result(result);
					mysql_close(conn);
				}

				if (!(temp1.Compare(temp7)))  // temp1에 담긴 카드 정보와 사전에 저장했던 번호를 비교하여 같으면 0을 호출
				{
					sprintf_s(query, 1024, "SELECT * FROM book WHERE num = 3");  //15693 - 2ad768cb500104e0

					if (mysql_query(conn, query))  // Send Query
					{
						std::cout << "SELECT Fail" << std::endl;
					}

					result = mysql_store_result(conn);
					int fields = mysql_num_fields(result);    // 필드 갯수 구함  

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];  // DB에서 불러온 정보를 각각의 그릇에 담는 역할
						cbeego = row[2];
						cimage = row[3];
						cwitch = row[4];

						image.Load(cimage);  // 사진 출력 부분
						m_strWitch = cwitch;  // 위치 출력 부분
						m_strBeego = cbeego;  // 비고 출력 부분
						m_strNamE = cname;  // 이름 출력 부분
						image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);  //이미지를 픽쳐 컨트롤 크기로 조정
						ReleaseDC(dc);//DC 해제
						UpdateData(FALSE);  // 변화된 정보를 한번에 출력되도록 하기
					}
					mysql_free_result(result);
					mysql_close(conn);
				}

				if (!(temp1.Compare(temp8)))  // temp1에 담긴 카드 정보와 사전에 저장했던 번호를 비교하여 같으면 0을 호출
				{
					sprintf_s(query, 1024, "SELECT * FROM book WHERE num = 4");  //15693 - 3efe68cb500104e0

					if (mysql_query(conn, query))  // Send Query
					{
						std::cout << "SELECT Fail" << std::endl;
					}

					result = mysql_store_result(conn);
					int fields = mysql_num_fields(result);    // 필드 갯수 구함  

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];  // DB에서 불러온 정보를 각각의 그릇에 담는 역할
						cbeego = row[2];
						cimage = row[3];
						cwitch = row[4];

						image.Load(cimage);  // 사진 출력 부분
						m_strWitch = cwitch;  // 위치 출력 부분
						m_strBeego = cbeego;  // 비고 출력 부분
						m_strNamE = cname;  // 이름 출력 부분
						image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);  //이미지를 픽쳐 컨트롤 크기로 조정
						ReleaseDC(dc);//DC 해제
						UpdateData(FALSE);  // 변화된 정보를 한번에 출력되도록 하기
					}
					mysql_free_result(result);
					mysql_close(conn);
				}
				PlaySoundW(_T("책넘기는소리.wav"), NULL, SND_FILENAME | SND_ASYNC);  // 소리 재생 부분
			}
		}
	}
}

void CRFIDDlg::OnReadContinue()
{
	if (flag_r == 0)
	{
		m_isWorkingThread = true;
		m_pThread = AfxBeginThread(Load, this);
	}
	else
	{
		m_isWorkingThread = false;
		WaitForSingleObject(m_pThread->m_hThread, 5000);
	}
}

void CRFIDDlg::OnDisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		// 무선파워를 끊어요.
	is_RfOff(ftHandle);
	//USB 포트를 Close
	if (is_Close(ftHandle) == IS_OK)
	{
		printf("연결을 닫습니다. ");
	}
}

UINT Load(LPVOID LpData)
{
	CRFIDDlg* target = (CRFIDDlg*)(LpData);

	while (target->m_isWorkingThread)
	{
		Sleep(2000);

		PostMessage(target->m_hWnd, ON_READ_ONCE, NULL, NULL);
	}
	return 0;
}

LRESULT CRFIDDlg::UpdateCount(WPARAM wParam, LPARAM lParam)
{
	// 이미지 출력 부분
	CRect rect;  // 픽쳐 컨트롤의 크기를 저장할 CRect 객체
	m_picture_control.GetWindowRect(rect);  // GetWindowRect를 사용해서 픽쳐 컨트롤의 크기를 받는다.
	CDC* dc;  // 픽쳐 컨트롤의 DC를 가져올  CDC 포인터
	dc = m_picture_control.GetDC();  // 픽쳐 컨트롤의 DC를 얻는다.
	CImage image;  // 불러오고 싶은 이미지를 로드할 CImage

	if (flag_r == 0)
	{
		// table명 : member, book, wine, cow, song 등 수정만 하면 모두 불러올 수 있게 설계
		if (is_WriteReadCommand(ftHandle, CM1_ISO14443AB, CM2_ISO14443A_ACTIVE + BUZZER_ON,  //ISO14443A모드로 읽기
			writeLength, wirteData, &readLength, readData) == IS_OK ||  // or로 모두 읽을 수 있도록 함
			is_WriteReadCommand(ftHandle, CM1_ISO15693, CM2_ISO15693_ACTIVE + BUZZER_ON,  //ISO15693모드로 읽기
				writeLength, wirteData, &readLength, readData) == IS_OK)
		{
			// DB 부분
			unsigned int timeout_sec = 1;
			char query[1024];
			MYSQL* conn;
			conn = mysql_init(NULL);
			mysql_options(conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout_sec);
			MYSQL_ROW row;
			MYSQL_RES* result;
			MYSQL* conn_result;
			conn_result = mysql_real_connect(conn, "127.0.0.1", "root", "qkqh123*", "mysql", 3306, NULL, 0);

			for (int i = 0; i < readLength; i++)  // 카드 정보를 temp1이라는 변수에 저장
			{
				temp.Format(_T("%02x "), readData[i]);
				temp1 += temp;
			}
			printf("\n");

			if (NULL == conn_result)
			{
				std::cout << "DB Connection Fail" << std::endl;
			}
			else
			{
				std::cout << "DB Connection Success" << std::endl;

				if (!(temp1.Compare(temp5)))  // temp1에 담긴 카드 정보와 사전에 저장했던 번호를 비교하여 같으면 0을 호출
				{
					sprintf_s(query, 1024, "SELECT * FROM book WHERE num = 1");  //14443 - 81935120

					if (mysql_query(conn, query))  // Send Query
					{
						std::cout << "SELECT Fail" << std::endl;
					}

					result = mysql_store_result(conn);
					int fields = mysql_num_fields(result);    // 필드 갯수 구함  

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];  // DB에서 불러온 정보를 각각의 그릇에 담는 역할
						cbeego = row[2];
						cimage = row[3];
						cwitch = row[4];

						image.Load(cimage);  // 사진 출력 부분
						m_strWitch = cwitch;  // 위치 출력 부분
						m_strBeego = cbeego;  // 비고 출력 부분
						m_strNamE = cname;  // 이름 출력 부분
						image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);  //이미지를 픽쳐 컨트롤 크기로 조정
						ReleaseDC(dc);//DC 해제
						UpdateData(FALSE);  // 변화된 정보를 한번에 출력되도록 하기
					}
					mysql_free_result(result);
					mysql_close(conn);
				}

				if (!(temp1.Compare(temp6)))  // temp1에 담긴 카드 정보와 사전에 저장했던 번호를 비교하여 같으면 0을 호출
				{
					sprintf_s(query, 1024, "SELECT * FROM book WHERE num = 2");  //14443 - f10c9daf

					if (mysql_query(conn, query))  // Send Query
					{
						std::cout << "SELECT Fail" << std::endl;
					}

					result = mysql_store_result(conn);
					int fields = mysql_num_fields(result);    // 필드 갯수 구함  

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];  // DB에서 불러온 정보를 각각의 그릇에 담는 역할
						cbeego = row[2];
						cimage = row[3];
						cwitch = row[4];

						image.Load(cimage);  // 사진 출력 부분
						m_strWitch = cwitch;  // 위치 출력 부분
						m_strBeego = cbeego;  // 비고 출력 부분
						m_strNamE = cname;  // 이름 출력 부분
						image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);  //이미지를 픽쳐 컨트롤 크기로 조정
						ReleaseDC(dc);//DC 해제
						UpdateData(FALSE);  // 변화된 정보를 한번에 출력되도록 하기
					}
					mysql_free_result(result);
					mysql_close(conn);
				}

				if (!(temp1.Compare(temp7)))  // temp1에 담긴 카드 정보와 사전에 저장했던 번호를 비교하여 같으면 0을 호출
				{
					sprintf_s(query, 1024, "SELECT * FROM book WHERE num = 3");  //15693 - 2ad768cb500104e0

					if (mysql_query(conn, query))  // Send Query
					{
						std::cout << "SELECT Fail" << std::endl;
					}

					result = mysql_store_result(conn);
					int fields = mysql_num_fields(result);    // 필드 갯수 구함  

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];  // DB에서 불러온 정보를 각각의 그릇에 담는 역할
						cbeego = row[2];
						cimage = row[3];
						cwitch = row[4];

						image.Load(cimage);  // 사진 출력 부분
						m_strWitch = cwitch;  // 위치 출력 부분
						m_strBeego = cbeego;  // 비고 출력 부분
						m_strNamE = cname;  // 이름 출력 부분
						image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);  //이미지를 픽쳐 컨트롤 크기로 조정
						ReleaseDC(dc);//DC 해제
						UpdateData(FALSE);  // 변화된 정보를 한번에 출력되도록 하기
					}
					mysql_free_result(result);
					mysql_close(conn);
				}

				if (!(temp1.Compare(temp8)))  // temp1에 담긴 카드 정보와 사전에 저장했던 번호를 비교하여 같으면 0을 호출
				{
					sprintf_s(query, 1024, "SELECT * FROM book WHERE num = 4");  //15693 - 3efe68cb500104e0

					if (mysql_query(conn, query))  // Send Query
					{
						std::cout << "SELECT Fail" << std::endl;
					}

					result = mysql_store_result(conn);
					int fields = mysql_num_fields(result);    // 필드 갯수 구함  

					while (row = mysql_fetch_row(result))     // 모든 레코드 탐색
					{
						cname = row[1];  // DB에서 불러온 정보를 각각의 그릇에 담는 역할
						cbeego = row[2];
						cimage = row[3];
						cwitch = row[4];

						image.Load(cimage);  // 사진 출력 부분
						m_strWitch = cwitch;  // 위치 출력 부분
						m_strBeego = cbeego;  // 비고 출력 부분
						m_strNamE = cname;  // 이름 출력 부분
						image.StretchBlt(dc->m_hDC, 0, 0, rect.Width(), rect.Height(), SRCCOPY);  //이미지를 픽쳐 컨트롤 크기로 조정
						ReleaseDC(dc);//DC 해제
						UpdateData(FALSE);  // 변화된 정보를 한번에 출력되도록 하기
					}
					mysql_free_result(result);
					mysql_close(conn);
				}
				PlaySoundW(_T("책넘기는소리.wav"), NULL, SND_FILENAME | SND_ASYNC);  // 소리 재생 부분
			}
		}
	}
	return 0;
}

afx_msg LRESULT CRFIDDlg::OnMymsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}
