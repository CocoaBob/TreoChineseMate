VERSION 5.00
Begin VB.Form frmMain 
   Caption         =   "���������"
   ClientHeight    =   6630
   ClientLeft      =   60
   ClientTop       =   750
   ClientWidth     =   5430
   LinkTopic       =   "Form1"
   ScaleHeight     =   442
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   362
   StartUpPosition =   2  '��Ļ����
   Begin VB.Frame Frame1 
      Caption         =   "��ǰ�ؼ�: �绰�����"
      Height          =   1335
      Left            =   120
      TabIndex        =   9
      Top             =   5040
      Width           =   5175
      Begin VB.CommandButton Command1 
         Caption         =   "���..."
         Height          =   375
         Left            =   4200
         TabIndex        =   22
         Top             =   720
         Width           =   855
      End
      Begin VB.TextBox Text6 
         Enabled         =   0   'False
         Height          =   375
         Left            =   1080
         TabIndex        =   21
         Top             =   720
         Width           =   3015
      End
      Begin VB.TextBox Text5 
         Alignment       =   1  'Right Justify
         Enabled         =   0   'False
         Height          =   375
         Left            =   4440
         TabIndex        =   19
         Text            =   "0"
         Top             =   240
         Width           =   495
      End
      Begin VB.TextBox Text4 
         Alignment       =   1  'Right Justify
         Height          =   375
         Left            =   3240
         TabIndex        =   17
         Text            =   "20"
         Top             =   240
         Width           =   495
      End
      Begin VB.TextBox Text3 
         Alignment       =   1  'Right Justify
         Height          =   375
         Left            =   2280
         TabIndex        =   15
         Text            =   "100"
         Top             =   240
         Width           =   495
      End
      Begin VB.TextBox Text2 
         Alignment       =   1  'Right Justify
         Height          =   375
         Left            =   1320
         TabIndex        =   13
         Text            =   "130"
         Top             =   240
         Width           =   495
      End
      Begin VB.TextBox Text1 
         Alignment       =   1  'Right Justify
         Height          =   375
         Left            =   480
         TabIndex        =   11
         Text            =   "60"
         Top             =   240
         Width           =   495
      End
      Begin VB.Label Label6 
         Caption         =   "ͼƬλ��:"
         Enabled         =   0   'False
         Height          =   375
         Left            =   240
         TabIndex        =   20
         Top             =   840
         Width           =   855
      End
      Begin VB.Label Label5 
         Caption         =   "����:"
         Enabled         =   0   'False
         Height          =   255
         Left            =   3960
         TabIndex        =   18
         Top             =   360
         Width           =   615
      End
      Begin VB.Label Label4 
         Caption         =   "��:"
         Height          =   255
         Left            =   2880
         TabIndex        =   16
         Top             =   360
         Width           =   375
      End
      Begin VB.Label Label3 
         Caption         =   "��:"
         Height          =   255
         Left            =   1920
         TabIndex        =   14
         Top             =   360
         Width           =   375
      End
      Begin VB.Label Label2 
         Caption         =   "Y:"
         Height          =   255
         Left            =   1080
         TabIndex        =   12
         Top             =   360
         Width           =   255
      End
      Begin VB.Label Label1 
         Caption         =   "X:"
         Height          =   255
         Left            =   240
         TabIndex        =   10
         Top             =   360
         Width           =   255
      End
   End
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   4575
      Left            =   360
      Picture         =   "frmMain.frx":0000
      ScaleHeight     =   305
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   302
      TabIndex        =   1
      Top             =   240
      Width           =   4530
      Begin VB.PictureBox Picture8 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         ForeColor       =   &H80000008&
         Height          =   375
         Left            =   3960
         ScaleHeight     =   345
         ScaleWidth      =   465
         TabIndex        =   8
         Top             =   3960
         Width           =   495
      End
      Begin VB.PictureBox Picture7 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         ForeColor       =   &H80000008&
         Height          =   375
         Left            =   3120
         ScaleHeight     =   345
         ScaleWidth      =   705
         TabIndex        =   7
         Top             =   3960
         Width           =   735
      End
      Begin VB.PictureBox Picture6 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         ForeColor       =   &H80000008&
         Height          =   375
         Left            =   2160
         ScaleHeight     =   345
         ScaleWidth      =   705
         TabIndex        =   6
         Top             =   3960
         Width           =   735
      End
      Begin VB.PictureBox Picture5 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         ForeColor       =   &H80000008&
         Height          =   615
         Left            =   840
         ScaleHeight     =   585
         ScaleWidth      =   3465
         TabIndex        =   5
         Top             =   3240
         Width           =   3495
      End
      Begin VB.PictureBox Picture4 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         ForeColor       =   &H80000008&
         Height          =   375
         Left            =   120
         ScaleHeight     =   345
         ScaleWidth      =   1905
         TabIndex        =   4
         Top             =   3960
         Width           =   1935
      End
      Begin VB.PictureBox Picture3 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         ForeColor       =   &H80000008&
         Height          =   735
         Left            =   120
         ScaleHeight     =   705
         ScaleWidth      =   585
         TabIndex        =   3
         Top             =   3240
         Width           =   615
      End
      Begin VB.PictureBox Picture2 
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         ForeColor       =   &H80000008&
         Height          =   2655
         Left            =   120
         ScaleHeight     =   2625
         ScaleWidth      =   4185
         TabIndex        =   2
         Top             =   120
         Width           =   4215
      End
   End
   Begin VB.Frame famDesignView 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   4800
      Left            =   240
      TabIndex        =   0
      Top             =   120
      Width           =   4800
   End
   Begin VB.Menu menuFile 
      Caption         =   "�ļ�(&F)"
      Begin VB.Menu menuOpen 
         Caption         =   "��"
      End
      Begin VB.Menu menuSave 
         Caption         =   "����"
      End
   End
   Begin VB.Menu menuHelp 
      Caption         =   "����(&H)"
      Begin VB.Menu menuReadme 
         Caption         =   "���涨��˵��"
      End
      Begin VB.Menu menuAbout 
         Caption         =   "����"
      End
   End
End
Attribute VB_Name = "frmMain"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Dim curDragControl, offsetX, offsetY As Integer

Private Sub famDesignView_DragDrop(Source As Control, X As Single, Y As Single)
    Dim tempX, tempY As Integer
    
    If curDragControl >= 0 Then
        '��ȡ���겢����
        tempX = X - offsetX + famDesignView.Left
        tempY = Y - offsetY + famDesignView.Top
        If tempX < famDesignView.Left Then
            tempX = famDesignView.Left
        End If
        If tempY < famDesignView.Top Then
            tempY = famDesignView.Top
        End If
        
        '���ÿؼ�����
        If curDragControl = 0 Then
            picBG.Left = tempX
            picBG.Top = tempY
        End If
        
        '��������ؼ����
        curDragControl = -1
    End If
End Sub

Private Sub Form_Load()
    curDragControl = -1
    'picBG.Picture = LoadPicture("E:\Development\PALM\WorkSpace\EasyPhone\UIDesigner\����.bmp")
    'picBG.Width = picBG.Picture.Width
    'picBG.ZOrder (0)
    'picBG.Width = 320
    'Image1.Picture = LoadPicture("E:\Development\PALM\WorkSpace\EasyPhone\UIDesigner\����.bmp")
    'Image1.Width = 320
End Sub

Private Sub picBG_DragOver(Source As Control, X As Single, Y As Single, State As Integer)
    If curDragControl <> 0 Then
        offsetX = X
        offsetY = Y
    End If
    curDragControl = 0
End Sub

