#include <PalmOS.h>
#include <PalmOSGlue.h>
#include <68K\Hs.h>
#include <Helper.h>
#include <HelperServiceClass.h>
#include <palmOnePhotoCommon.h>
#include <palmOnePhoto.h>

#include <hsKeyCodes.h>

#include "TreoChineseMate.h"
#include "TreoChineseMate_Rsc.h"

static UInt16 exportImageToCard(SettingType *settingP)
{
	UInt16					error = errNone;
	BitmapType				*bmpP;
	UInt32					bmpBitsSize, bmpHeaderSize;
	void					*bmpBitsP;
	
	UInt16					photoLibRef;
	PalmPhotoFileLocation	photoFile;
	PalmPhotoImageInfo		photoInfo;
	PalmPhotoHandle			photoH = NULL;
	RectangleType			winRect;
	PalmPhotoDisplayParam	displayParam;
	
	UInt16					volRef = 0;
	UInt32					fileRef = 0;
	UInt32					volIterator = vfsIteratorStart;
	
	while (volIterator != vfsIteratorStop)
	{
		VFSVolumeEnumerate(&volRef, &volIterator);
	}
	
	if (volRef > 0)
	{
		error = SysLibLoad(PalmPhotoLibTypeID, PalmPhotoLibCreatorID, &photoLibRef);
		
		if (error == errNone)
		{
			error = PalmPhotoLibOpen(photoLibRef);
			
			if (error == errNone)
			{
				photoFile.fileLocationType = palmPhotoVFSLocation;
				StrCopy(photoFile.file.VFSFile.name, settingP->skin.bmpPath);
				photoFile.file.VFSFile.volumeRef = volRef;
				
				photoH = PalmPhotoOpenImageV2(photoLibRef, &photoFile);
				
				if (photoH)
				{
					error = PalmPhotoGetImageInfo(photoLibRef, photoH, &photoInfo);
					
					if (error == errNone)
					{
						winRect.topLeft.x = 0;
						winRect.topLeft.y = 0;
						winRect.extent.x = photoInfo.width;
						winRect.extent.y = photoInfo.height;
						
						bmpP = BmpCreate(photoInfo.width, photoInfo.height, 16, NULL, &error);
						
						if (error == errNone)
						{
							MemSet(&displayParam, sizeof(PalmPhotoDisplayParam), 0x00);
							displayParam.winH = WinCreateBitmapWindow(bmpP, &error);
							
							if (error == errNone)
							{
								displayParam.rect = winRect;
								
								error = PalmPhotoDisplayImageToSizeV3(photoLibRef, photoH, &displayParam);
								if (error == errNone)
								{
									BmpGetSizes(bmpP, &bmpBitsSize, &bmpHeaderSize);
									bmpBitsP = BmpGetBits(bmpP);
									
									//保存到储存卡
									if (VFSFileOpen(volRef, "/600Skin.bin", vfsModeCreate, &fileRef) == errNone)
									{
										VFSFileWrite(fileRef, bmpBitsSize, bmpBitsP, NULL);
										VFSFileClose(fileRef);
									}
								}
								
								WinDeleteWindow(displayParam.winH, false);
							}
							
							BmpDelete(bmpP);
						}
					}
					
					PalmPhotoCloseImageV2(photoLibRef, photoH);
				}
			}
			PalmPhotoLibClose(photoLibRef);
		}
		
		SysLibRemove(photoLibRef);
	}
	else
	{
		error = 1;
	}
	
	return error;
}

static UInt16 loadSkinToFeature(SettingType *settingP)
{
	UInt16					error;
	
	UInt32					volIterator = vfsIteratorStart;
	UInt16					volRef;
	
	UInt16					photoLibRef;
	PalmPhotoFileLocation	photoFile;
	PalmPhotoImageInfo		photoInfo;
	PalmPhotoHandle			photoH = NULL;
	RectangleType			winRect;
	PalmPhotoDisplayParam	displayParam;
	
	BitmapType				*bmpP;
	void					*bmpBitsP;
	UInt32					bmpHeaderSize;
	UInt32					bmpBitsSize;
	
	FormType				*frmLoadP	= FrmInitForm(frmLoading);
	
	//FrmSetActiveForm(frmLoadP);
	FrmDrawForm(frmLoadP);
	
	while (volIterator != vfsIteratorStop)
	{
		VFSVolumeEnumerate(&volRef, &volIterator);
	}
	
	if (volRef > 0)
	{
		if (settingP->treoModel != TREO_600)
		{
			error = SysLibLoad(PalmPhotoLibTypeID, PalmPhotoLibCreatorID, &photoLibRef);
			
			if (error == errNone)
			{
				error = PalmPhotoLibOpen(photoLibRef);
				
				if (error == errNone)
				{
					photoFile.fileLocationType = palmPhotoVFSLocation;
					StrCopy(photoFile.file.VFSFile.name, settingP->skin.bmpPath);
					photoFile.file.VFSFile.volumeRef = volRef;
					
					photoH = PalmPhotoOpenImageV2(photoLibRef, &photoFile);
					
					if (photoH)
					{
						error = PalmPhotoGetImageInfo(photoLibRef, photoH, &photoInfo);
						
						if (error == errNone)
						{
							winRect.topLeft.x = 0;
							winRect.topLeft.y = 0;
							winRect.extent.x = photoInfo.width;
							winRect.extent.y = photoInfo.height;
							
							bmpP = BmpCreate(photoInfo.width, photoInfo.height, 16, NULL, &error);
							
							if (error == errNone)
							{
								MemSet(&displayParam, sizeof(PalmPhotoDisplayParam), 0x00);
								displayParam.winH = WinCreateBitmapWindow(bmpP, &error);
								
								if (error == errNone)
								{
									displayParam.rect = winRect;
									
									error = PalmPhotoDisplayImageToSizeV3(photoLibRef, photoH, &displayParam);
									if (error == errNone)
									{
										BmpGetSizes(bmpP, &bmpBitsSize, &bmpHeaderSize);
										
										error = FtrPtrNew('EzPH', 1, bmpBitsSize, &settingP->skin.bmpBitsP);
										if (error == errNone)
										{
											bmpBitsP = BmpGetBits(bmpP);
											DmWrite(settingP->skin.bmpBitsP, 0, bmpBitsP, bmpBitsSize);
											settingP->skin.bmpWidth = photoInfo.width;
											settingP->skin.bmpHeight = photoInfo.height;
											settingP->skin.bmpDensity = kDensityDouble;
											settingP->skin.bmpBitsSize = bmpBitsSize;
										}
									}
									
									WinDeleteWindow(displayParam.winH, false);
								}
								
								BmpDelete(bmpP);
							}
						}
						
						PalmPhotoCloseImageV2(photoLibRef, photoH);
					}
				}
				PalmPhotoLibClose(photoLibRef);
			}
			
			SysLibRemove(photoLibRef);
		}
		else
		{
			UInt32		VFSFileRef;
			
			error = VFSFileOpen(volRef, settingP->skin.bmpPath, vfsModeRead, &VFSFileRef);
			
			if (error == errNone)
			{
				VFSFileSize(VFSFileRef, &bmpBitsSize);
				
				error = FtrPtrNew('EzPH', 1, bmpBitsSize, &settingP->skin.bmpBitsP);
				
				if (error == errNone)
				{
					VFSFileSeek(VFSFileRef, vfsOriginBeginning, 0);
					
					VFSFileReadData(VFSFileRef, bmpBitsSize, settingP->skin.bmpBitsP, 0, NULL);
					
					VFSFileClose(VFSFileRef);
					
					settingP->skin.bmpWidth = settingP->skin.frmContacts.bounds.extent.x;
					settingP->skin.bmpHeight = settingP->skin.frmContacts.bounds.extent.x;
					settingP->skin.bmpDensity = kDensityLow;
					settingP->skin.bmpBitsSize = bmpBitsSize;
				}
			}
		}
	}
	else
	{
		error = 1;
	}
	
	FrmEraseForm(frmLoadP);
	FrmDeleteForm(frmLoadP);
	
	return error;
}

static void getData(SkinSettingType *skin, Char *data, UInt8 currentObject, UInt8 col)
{
	ObjectSettingType		*object;
	
	switch (currentObject)
	{
		case 0:
		{
			object = &skin->frmContacts;
			break;
		}
		case 1:
		{
			object = &skin->lstContacts;
			break;
		}
		case 2:
		{
			object = &skin->gdtDialButton;
			break;
		}
		case 3:
		{
			object = &skin->fldPhoneNumber;
			break;
		}
		case 4:
		{
			object = &skin->gdtPreFixTrigger;
			break;
		}
		case 5:
		{
			object = &skin->gdtOperationButton;
			break;
		}
		case 6:
		{
			object = &skin->gdtSMSButton;
			break;
		}
		case 7:
		{
			object = &skin->gdtHelpButton;
			break;
		}
	}
	
	switch (col)
	{
		case 0:
		{
			object->bounds.topLeft.x = StrAToI(data);
			/*if (currentObject > 0)
			{
				object->x += skin->frmContacts.x;
			}*/
			break;
		}
		case 1:
		{
			object->bounds.topLeft.y = StrAToI(data);
			/*if (currentObject > 0)
			{
				object->y += skin->frmContacts.y;
			}*/
			break;
		}
		case 2:
		{
			object->bounds.extent.x = StrAToI(data);
			break;
		}
		case 3:
		{
			object->bounds.extent.y = StrAToI(data);
			break;
		}
		case 4:
		{
			object->other = StrAToI(data);
			break;
		}
	}
}

static UInt16 previewImage(Char *imagePathP, SkinSettingType *skin, UInt16 volRef, BitmapType **bmpPP, UInt32 *bmpSize, SettingType *settingP)
{
	UInt16					error = errNone;
	
	UInt16					photoLibRef;
	PalmPhotoFileLocation	photoFile;
	PalmPhotoImageInfo		photoInfo;
	PalmPhotoHandle			photoH = NULL;
	RectangleType			winRect;
	RectangleType			previewRect;
	PalmPhotoDisplayParam	displayParam;
	WinHandle				tmpWinH;
	
	if (settingP->treoModel != TREO_600)
	{
		error = SysLibLoad(PalmPhotoLibTypeID, PalmPhotoLibCreatorID, &photoLibRef);
		
		if (error == errNone)
		{
			error = PalmPhotoLibOpen(photoLibRef);
			
			if (error == errNone)
			{
				//显示图片
				photoFile.fileLocationType = palmPhotoVFSLocation;
				StrCopy(photoFile.file.VFSFile.name, imagePathP);
				photoFile.file.VFSFile.volumeRef = volRef;
				
				photoH = PalmPhotoOpenImageV2(photoLibRef, &photoFile);
				
				if (photoH)
				{
					error = PalmPhotoGetImageInfo(photoLibRef, photoH, &photoInfo);
					
					if (error == errNone)
					{
						//显示预览图
						(*bmpPP) = BmpCreate(photoInfo.width, photoInfo.height, 16, NULL, &error);
						
						if (error == errNone)
						{
							winRect.topLeft.x = 0;
							winRect.topLeft.y = 0;
							winRect.extent.x = photoInfo.width;
							winRect.extent.y = photoInfo.height;
							
							MemSet(&displayParam, sizeof(PalmPhotoDisplayParam), 0x00);
							displayParam.rect = winRect;
							displayParam.winH = WinCreateBitmapWindow((*bmpPP), &error);
							
							if(error == errNone)
							{
								error = PalmPhotoDisplayImageToSizeV3(photoLibRef, photoH, &displayParam);
								
								if (error == errNone)
								{
									tmpWinH = displayParam.winH;
									
									MemSet(&displayParam, sizeof(PalmPhotoDisplayParam), 0x00);
									previewRect.topLeft.x = 64;
									previewRect.topLeft.y = 34;
									previewRect.extent.x = 200;
									previewRect.extent.y = 200;
									displayParam.rect = previewRect;
									PalmPhotoDisplayImageToSizeV3(photoLibRef, photoH, &displayParam);
									
									displayParam.winH = tmpWinH;
								}
								
								WinDeleteWindow(displayParam.winH, false);
							}
						}
					}
					
					PalmPhotoCloseImageV2(photoLibRef, photoH);
				}
				
				PalmPhotoLibClose(photoLibRef);
			}
		}
		
		SysLibRemove(photoLibRef);
	}
	else
	{
		UInt32					VFSFileRef;
		UInt32					bmpBitsSize;
		void					*bmpBitsP;
		
		error = VFSFileOpen(volRef, imagePathP, vfsModeRead, &VFSFileRef);
		
		if (error == errNone)
		{
			VFSFileSize(VFSFileRef, &bmpBitsSize);
			
			bmpBitsP = MemGluePtrNew(bmpBitsSize);
			if (bmpBitsP)
			{
				VFSFileSeek(VFSFileRef, vfsOriginBeginning, 0);
				
				if (VFSFileRead(VFSFileRef, bmpBitsSize, bmpBitsP, NULL) == errNone)
				{
					(*bmpPP) = BmpCreate(skin->frmContacts.bounds.extent.x, skin->frmContacts.bounds.extent.y, 16, NULL, &error);
					
					if (error == errNone)
					{
						BitmapTypeV3 *bmpV3P = BmpCreateBitmapV3((*bmpPP), kDensityDouble, bmpBitsP, NULL);
						WinDrawBitmap((BitmapType *)bmpV3P, 30, 15);
						BmpDelete((BitmapType *)bmpV3P);
					}
				}
				
				MemPtrFree(bmpBitsP);
			}
			
			VFSFileClose(VFSFileRef);
		}
	}
	
	return error;
}

static void loadSkin(Char *skinNameP, SettingType *settingP)
{
	UInt16			volRef = 0;
	UInt32			fileRef = 0;
	UInt32			volIterator = vfsIteratorStart;
	Char			*skinFullPathP;
	UInt32			fileSize;
	Char			*fileBuffer;
	SkinSettingType	skin;
	UInt32			i = 0;
	UInt16			j = 0;
	Char			*tmp;
	Char			data[4];
	UInt8			object = 0;
	UInt8			col = 0;
	UInt16			imgLibRef = 0;
	BitmapType		*bmpP = NULL;
	UInt32			bmpSize = 0;
	FormType		*msgFrmP;
	
	//取卡指针
	while (volIterator != vfsIteratorStop)
	{
		VFSVolumeEnumerate(&volRef, &volIterator);
	}
	
	if (volRef > 0)
	{
		skinFullPathP = (Char *)MemPtrNew(10240);
		StrCopy(skinFullPathP, "/PALM/Programs/EasyPhone/");
		StrCat(skinFullPathP, skinNameP);
		StrCat(skinFullPathP, ".ini");
		
		if (VFSFileOpen(volRef, skinFullPathP, vfsModeRead, &fileRef) == errNone)
		{
			VFSFileSize(fileRef, &fileSize);
			fileBuffer = (Char *)MemPtrNew(fileSize);
			VFSFileSeek(fileRef, vfsOriginBeginning, 0);
			VFSFileRead(fileRef, fileSize, (void *)fileBuffer, NULL);
			VFSFileClose(fileRef);
			
			tmp = fileBuffer;
			MemSet(data, 4, 0x00);
			
			while (i < fileSize && object < 8)
			{
				while ((UInt8)(*tmp) != 0x0A && i < fileSize)
				{
					if ((UInt8)(*tmp) != 0x0D)
					{
						data[j] = *tmp;
						j ++;
						
						i ++; tmp ++;
						
						if ((*tmp) == ',')
						{
							getData(&skin, data, object, col);
							col ++;
							tmp ++;
							i ++;
							j = 0;
						}
					}
					else
					{
						tmp ++; i ++;
					}
				}
				getData(&skin, data, object, col);
				
				col = 0;
				object ++;
				j = 0;
				
				if (i < fileSize)
				{
					tmp ++; i ++;
				}
			}
			MemPtrFree(fileBuffer);
			
			//添加背景图
			StrCopy(skinFullPathP, "/PALM/Programs/EasyPhone/");
			StrCat(skinFullPathP, skinNameP);
			if (settingP->treoModel != TREO_600)
			{
				StrCat(skinFullPathP, ".bmp");
			}
			else
			{
				StrCat(skinFullPathP, ".bin");
			}
			
			if (previewImage(skinFullPathP, &skin, volRef, &bmpP, &bmpSize, settingP) == errNone)
			{
				//保存到设置
				settingP->skin = skin;
				StrCopy(settingP->skin.bmpPath, skinFullPathP);
				
				msgFrmP = FrmInitForm(frmLoadOK);
				FrmDrawForm(msgFrmP);
				FrmSetActiveForm(msgFrmP);
				SysTaskDelay(SysTicksPerSecond() / 2);
				FrmReturnToForm(0);
				BmpDelete(bmpP);
			}
			else
			{
				FrmAlert(alertFileError);
			}
		}
		else
		{
			FrmAlert(alertFileError);
		}
		
		MemPtrFree(skinFullPathP);
	}
}

static Char **getSkinList(UInt16 *listCount)
{
	Char			**skinList = NULL;
	UInt16			volRef = 0;
	UInt32			dirRef = 0;
	UInt32			volIterator = vfsIteratorStart;
	UInt32			dirIterator = vfsIteratorStart;
	FileInfoType	fileInfo;
	Char			*fileExtName = NULL;
	UInt16			i;
	const Char		*skinPath = "/PALM/Programs/EasyPhone/";
	
	//取卡指针
	while (volIterator != vfsIteratorStop)
	{
		VFSVolumeEnumerate(&volRef, &volIterator);
	}
	
	//VFSExportDatabaseToFile(volRef, "/低分皮肤.pdb", 0, DmFindDatabase(0, "EasyPhone Skin"));
	
	if (volRef > 0)
	{
		if (VFSFileOpen(volRef, skinPath, vfsModeReadWrite, &dirRef) == errNone) //打开 /PALM/Programs/EasyPhone
		{
			fileInfo.nameP = (Char *)MemPtrNew(32);
			fileInfo.nameBufLen = 32;
			
			//遍历文件夹
			while (dirIterator != vfsIteratorStop)
			{
				if (VFSDirEntryEnumerate(dirRef, &dirIterator, &fileInfo) == errNone)
				{
					fileExtName = StrChr(fileInfo.nameP, (WChar)'.'); //取扩展名
					if (fileExtName != NULL) //扩展名存在
					{
						if (StrNCaselessCompare(fileExtName, ".ini", 4) == 0) //是.ini，认为其是一个皮肤
						{
							(*listCount) ++; //皮肤计数器+1
						}
					}
				}
			}
			
			//创建文件列表
			skinList = (Char **)MemPtrNew((*listCount) * 4);
			dirIterator = vfsIteratorStart;
			i = 0;
			
			while (dirIterator != vfsIteratorStop)
			{
				if (VFSDirEntryEnumerate(dirRef, &dirIterator, &fileInfo) == errNone)
				{
					fileExtName = StrChr(fileInfo.nameP, (WChar)'.'); //取扩展名
					if (fileExtName != NULL) //扩展名存在
					{
						if (StrNCaselessCompare(fileExtName, ".ini", 4) == 0) //是.ini，认为其是一个皮肤
						{
							//分配列表单元的内存
							skinList[i] = (Char *)MemPtrNew(StrLen(fileInfo.nameP) - 3);
							MemSet(skinList[i], StrLen(fileInfo.nameP) - 3, 0x00);
							
							//获取名称
							StrNCopy(skinList[i], fileInfo.nameP, StrLen(fileInfo.nameP) - 4);
							i ++;
						}
					}
				}
			}
			VFSFileClose(dirRef);
			MemPtrFree(fileInfo.nameP);
		}
		else // /PALM/Programs/EasyPhone 不存在，创建它
		{
			VFSDirCreate(volRef, "/PALM/Programs/EasyPhone");
		}
	}
	
	return skinList;
}

static void setSkin(SettingType *settingP)
{
	EventType		event;
	UInt16			i;
	FormType		*frmSetSkinP = NULL;
	ListType		*lstSkinsP = NULL;
	Char			**skinListPP = NULL;
	UInt16			skinListCount = 0;
	Boolean			exit = false;
	RectangleType	rect;
	
	FrmPopupForm(frmSetSkin);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if (! SysHandleEvent(&event))
		{
			if (! MenuHandleEvent(0, &event, &i))
			{
				switch (event.eType)
				{
					case frmLoadEvent:
					{
						break;
					}
					case frmOpenEvent:
					{
						frmSetSkinP = FrmInitForm(frmSetSkin);
						FrmSetActiveForm(frmSetSkinP);
						
						lstSkinsP = getObjectPtr(frmSetSkinP, lstSkins);
						skinListPP = getSkinList(&skinListCount);
						
						if (skinListCount > 0)
						{
							LstSetListChoices(lstSkinsP, skinListPP, skinListCount);
						}
						LstSetSelection(lstSkinsP, noListSelection);
						
						FrmDrawForm(frmSetSkinP);
						
						rect.topLeft.x = 30;
						rect.topLeft.y = 15;
						rect.extent.x = 100;
						rect.extent.y = 100;
						
						WinDrawRectangleFrame(simpleFrame, &rect); 
						
						break;
					}
					case ctlSelectEvent:
					{
						switch (event.data.ctlSelect.controlID)
						{
							case btnLoadBMP:
							{
								if (LstGetSelection(lstSkinsP) != noListSelection && skinListCount > 0)
								{
									loadSkin(LstGetSelectionText(lstSkinsP, LstGetSelection(lstSkinsP)), settingP);
									//exit = true;
								}
								break;
							}
							case btnSetBMPDefault:
							{
								MemSet(&settingP->skin, sizeof(SkinSettingType), 0);
								settingP->skin.frmContacts.bounds.topLeft.x = 5;
								settingP->skin.frmContacts.bounds.topLeft.y = 7;
								settingP->skin.frmContacts.bounds.extent.x = 150;
								settingP->skin.frmContacts.bounds.extent.y = 149;
								settingP->skin.frmContacts.other = 0;
								//5,4,150,90,8
								settingP->skin.lstContacts.bounds.topLeft.x = 10;
								settingP->skin.lstContacts.bounds.topLeft.y = 11;
								settingP->skin.lstContacts.bounds.extent.x = 140;
								settingP->skin.lstContacts.bounds.extent.y = 90;
								settingP->skin.lstContacts.other = 8;
								//2,103,20,22,2
								settingP->skin.gdtDialButton.bounds.topLeft.x = 8;
								settingP->skin.gdtDialButton.bounds.topLeft.y = 113;
								settingP->skin.gdtDialButton.bounds.extent.x = 20;
								settingP->skin.gdtDialButton.bounds.extent.y = 25;
								settingP->skin.gdtDialButton.other = 2;
								//25,107,131,16,0
								settingP->skin.fldPhoneNumber.bounds.topLeft.x = 30;
								settingP->skin.fldPhoneNumber.bounds.topLeft.y = 113;
								settingP->skin.fldPhoneNumber.bounds.extent.x = 121;
								settingP->skin.fldPhoneNumber.bounds.extent.y = 25;
								settingP->skin.fldPhoneNumber.other = 0;
								//2,126,78,10,2
								settingP->skin.gdtPreFixTrigger.bounds.topLeft.x = 8;
								settingP->skin.gdtPreFixTrigger.bounds.topLeft.y = 140;
								settingP->skin.gdtPreFixTrigger.bounds.extent.x = 66;
								settingP->skin.gdtPreFixTrigger.bounds.extent.y = 12;
								settingP->skin.gdtPreFixTrigger.other = 2;
								//81,126,29,10,2
								settingP->skin.gdtOperationButton.bounds.topLeft.x = 75;
								settingP->skin.gdtOperationButton.bounds.topLeft.y = 140;
								settingP->skin.gdtOperationButton.bounds.extent.x = 29;
								settingP->skin.gdtOperationButton.bounds.extent.y = 12;
								settingP->skin.gdtOperationButton.other = 2;
								//111,126,29,10,2
								settingP->skin.gdtSMSButton.bounds.topLeft.x = 105;
								settingP->skin.gdtSMSButton.bounds.topLeft.y = 140;
								settingP->skin.gdtSMSButton.bounds.extent.x = 29;
								settingP->skin.gdtSMSButton.bounds.extent.y = 12;
								settingP->skin.gdtSMSButton.other = 2;
								//141,126,16,10,2
								settingP->skin.gdtHelpButton.bounds.topLeft.x = 135;
								settingP->skin.gdtHelpButton.bounds.topLeft.y = 140;
								settingP->skin.gdtHelpButton.bounds.extent.x = 16;
								settingP->skin.gdtHelpButton.bounds.extent.y = 12;
								settingP->skin.gdtHelpButton.other = 2;
								break;
							}
							case btnSetBMPOK:
							{
								exit = true;
								break;
							}
						}
						break;
					}
					case menuEvent:
					{
						if (event.data.menu.itemID == menuitemExportSkin)
						{
							if (settingP->treoModel != TREO_600)
							{
								if (exportImageToCard(settingP) != errNone)
								{
									FrmCustomAlert(alertInfo, "皮肤导出完成", "", "");
								}
								else
								{
									FrmCustomAlert(alertError, "皮肤导出失败", "", "");
								}
							}
							else
							{
								FrmCustomAlert(alertError, "无法在Treo 600上执行此操作", "", "");
							}
						}
						break;
					}
					default:
					{
						FrmHandleEvent(frmSetSkinP, &event);
					}
				}
			}
		}
	} while(event.eType != appStopEvent && exit == false);
	
	if (skinListCount > 0)
	{
		for (i = 0; i < skinListCount; i ++)
		{
			MemPtrFree(skinListPP[i]);
		}
		MemPtrFree(skinListPP);
	}
	
	FrmReturnToForm(0);
}

static void setPunc(SettingType *settingP)
{
	EventType		event;
	UInt16			i;
	Boolean			exit = false;
	FormType		*frmSetPuncP = NULL;
	FieldType		*fldPuncP[8];
	
	FrmPopupForm(frmSetPunc);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if (! SysHandleEvent(&event))
		{
			switch (event.eType)
			{
				case frmLoadEvent:
				{
					frmSetPuncP = FrmInitForm(frmSetPunc);
					FrmSetActiveForm(frmSetPuncP);
					break;
				}
				case frmOpenEvent:
				{
					FrmDrawForm(frmSetPuncP);
					for (i = fldSetPunc0; i <= fldSetPunc7; i ++)
					{
						fldPuncP[i - fldSetPunc0] = (FieldType *)getObjectPtr(frmSetPuncP, i);
						FldInsert(fldPuncP[i - fldSetPunc0], settingP->punc[i - fldSetPunc0], StrLen(settingP->punc[i - fldSetPunc0]));
					}
					break;
				}
				case ctlSelectEvent:
				{
					if (event.data.ctlSelect.controlID == btnSetPuncOK)
					{
						exit = true;
					}
					FrmHandleEvent(frmSetPuncP, &event);
					
					break;
				}
				default:
				{
					if (frmSetPuncP)
					{
						FrmHandleEvent(frmSetPuncP, &event);
					}
					break;
				}
			}
		}
	} while (event.eType != appStopEvent && ! exit);
	
	for (i = 0; i < 8; i ++)
	{
		if (FldGetTextPtr(fldPuncP[i]))
		{
			StrCopy(settingP->punc[i], FldGetTextPtr(fldPuncP[i]));
		}
		else
		{
			MemSet(settingP->punc[i], 9, 0);
		}
	}
	
	FrmReturnToForm(0);
}

static void setPreFix(SettingType *settingP)
{
	EventType		event;
	UInt16			i;
	Boolean			exit = false;
	FormType		*frmSetPreFixP = NULL;
	FieldType		*fldPreFix[6];
	
	FrmPopupForm(frmSetPreFix);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if (! SysHandleEvent(&event))
		{
			switch (event.eType)
			{
				case frmLoadEvent:
				{
					frmSetPreFixP = FrmInitForm(frmSetPreFix);
					FrmSetActiveForm(frmSetPreFixP);
					break;
				}
				case frmOpenEvent:
				{
					FrmDrawForm(frmSetPreFixP);
					for (i = fldPreFix0; i <= fldPreFix5; i ++)
					{
						fldPreFix[i - fldPreFix0] = (FieldType *)getObjectPtr(frmSetPreFixP, i);
						FldInsert(fldPreFix[i - fldPreFix0], settingP->preFix[i - fldPreFix0], StrLen(settingP->preFix[i - fldPreFix0]));
					}
					break;
				}
				case ctlSelectEvent:
				{
					if (event.data.ctlSelect.controlID == btnPreFixOK)
					{
						exit = true;
					}
					FrmHandleEvent(frmSetPreFixP, &event);
					
					break;
				}
				default:
				{
					if (frmSetPreFixP)
					{
						FrmHandleEvent(frmSetPreFixP, &event);
					}
					break;
				}
			}
		}
	} while (event.eType != appStopEvent && ! exit);
	
	for (i = 0; i < 6; i ++)
	{
		if (FldGetTextPtr(fldPreFix[i]))
		{
			StrCopy(settingP->preFix[i], FldGetTextPtr(fldPreFix[i]));
		}
		else
		{
			MemSet(settingP->preFix[i], 21, 0);
		}
	}
	
	FrmReturnToForm(0);
}

static void setAdvanceSetting(SettingType *settingP)
{
	EventType		event;
	Boolean			exit = false;
	FormType		*frmAdvanceSettingP				= NULL;
	ControlType		*cbShouldUseOldOperationTypeP	= NULL;
	ControlType		*ptriShowDetailTypeP			= NULL;
	ListType		*lstShowDetailTypeP				= NULL;
	ControlType		*ptriKeyHard2ActionP			= NULL;
	ListType		*lstKeyHard2ActionP				= NULL;
	ControlType		*ptriKeyHard3ActionP			= NULL;
	ListType		*lstKeyHard3ActionP				= NULL;
	
	FrmPopupForm(frmAdvanceSetting);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if (! SysHandleEvent(&event))
		{
			switch (event.eType)
			{
				case frmLoadEvent:
				{
					break;
				}
				case frmOpenEvent:
				{
					frmAdvanceSettingP = FrmInitForm(frmAdvanceSetting);
					FrmSetActiveForm(frmAdvanceSettingP);
					FrmDrawForm(frmAdvanceSettingP);
					
					cbShouldUseOldOperationTypeP = (ControlType *)getObjectPtr(frmAdvanceSettingP, cbShouldUseOldOperationType);
					
					ptriShowDetailTypeP = (ControlType *)getObjectPtr(frmAdvanceSettingP, ptriShowDetailType);
					lstShowDetailTypeP = (ListType *)getObjectPtr(frmAdvanceSettingP, lstShowDetailType);
					
					ptriKeyHard2ActionP = (ControlType *)getObjectPtr(frmAdvanceSettingP, ptriKeyHard2Action);
					lstKeyHard2ActionP = (ListType *)getObjectPtr(frmAdvanceSettingP, lstKeyHard2Action);
					
					ptriKeyHard3ActionP = (ControlType *)getObjectPtr(frmAdvanceSettingP, ptriKeyHard3Action);
					lstKeyHard3ActionP = (ListType *)getObjectPtr(frmAdvanceSettingP, lstKeyHard3Action);
					
					CtlSetValue(cbShouldUseOldOperationTypeP, settingP->shouldUseOldOperationType);
					
					LstSetSelection(lstShowDetailTypeP, settingP->showDetailType);
					CtlSetLabel(ptriShowDetailTypeP, LstGetSelectionText(lstShowDetailTypeP, settingP->showDetailType));
					
					LstSetSelection(lstKeyHard2ActionP, settingP->keyHard2Action);
					CtlSetLabel(ptriKeyHard2ActionP, LstGetSelectionText(lstKeyHard2ActionP, settingP->keyHard2Action));
					
					LstSetSelection(lstKeyHard3ActionP, settingP->keyHard3Action);
					CtlSetLabel(ptriKeyHard3ActionP, LstGetSelectionText(lstKeyHard3ActionP, settingP->keyHard3Action));
					
					break;
				}
				case ctlSelectEvent:
				{
					if (event.data.ctlSelect.controlID == btnAdvanceOK)
					{
						exit = true;
					}
					else if (event.data.ctlSelect.controlID == ptriShowDetailType)
					{
						LstPopupList(lstShowDetailTypeP);
						CtlSetLabel(ptriShowDetailTypeP, LstGetSelectionText(lstShowDetailTypeP, LstGetSelection(lstShowDetailTypeP)));
					}
					else if (event.data.ctlSelect.controlID == ptriKeyHard2Action)
					{
						LstPopupList(lstKeyHard2ActionP);
						CtlSetLabel(ptriKeyHard2ActionP, LstGetSelectionText(lstKeyHard2ActionP, LstGetSelection(lstKeyHard2ActionP)));
					}
					else if (event.data.ctlSelect.controlID == ptriKeyHard3Action)
					{
						LstPopupList(lstKeyHard3ActionP);
						CtlSetLabel(ptriKeyHard3ActionP, LstGetSelectionText(lstKeyHard3ActionP, LstGetSelection(lstKeyHard3ActionP)));
					}
					break;
				}
				default:
				{
					FrmDispatchEvent(&event);
					break;
				}
			}
		}
	} while (event.eType != appStopEvent && ! exit);
	
	settingP->shouldUseOldOperationType = CtlGetValue(cbShouldUseOldOperationTypeP);
	settingP->showDetailType = LstGetSelection(lstShowDetailTypeP);
	settingP->keyHard2Action = LstGetSelection(lstKeyHard2ActionP);
	settingP->keyHard3Action = LstGetSelection(lstKeyHard3ActionP);
	
	FrmReturnToForm(0);
}

static void getInstalledModule(ModuleInfomationType *easyPhoneInfo, ModuleInfomationType *easyInputInfo, ModuleInfomationType *easyContactInfo)
{
	DmSearchStateType	searchState;
	
	//查找“随手Phone”
	if (DmGetNextDatabaseByTypeCreator(true, &searchState, 'appl', 'EzPH', true, &easyPhoneInfo->cardNo, &easyPhoneInfo->dbId) == errNone)
	{
		easyPhoneInfo->isInstalled = true;
	}
	else
	{
		easyPhoneInfo->isInstalled = false;
	}
	
	//查找“输入易”
	if (DmGetNextDatabaseByTypeCreator(true, &searchState, 'appl', 'EzIP', true, &easyInputInfo->cardNo, &easyInputInfo->dbId) == errNone)
	{
		easyInputInfo->isInstalled = true;
	}
	else
	{
		easyInputInfo->isInstalled = false;
	}
	
	//查找“通讯录检索”
	if (DmGetNextDatabaseByTypeCreator(true, &searchState, 'appl', 'EzCT', true, &easyContactInfo->cardNo, &easyContactInfo->dbId) == errNone)
	{
		easyContactInfo->isInstalled = true;
	}
	else
	{
		easyContactInfo->isInstalled = false;
	}
}

static Char *getVersionString(UInt16 cardNo, LocalID dbId)
{
	MemHandle	resH			= NULL;
	Char		*tverStringP	= NULL;
	Char		*versionP		= NULL;
	DmOpenRef	dbRef			= DmOpenDatabase(cardNo, dbId, dmModeReadOnly);
	
	if (DmGetLastErr() == errNone)
	{
		resH = DmGet1Resource('tver', 1000);
		
		if (resH)
		{
			tverStringP = (Char *)MemHandleLock(resH);
			versionP = (Char *)MemPtrNew(StrLen(tverStringP) + 1);
			StrCopy(versionP, tverStringP);
			MemHandleUnlock(resH);
			DmReleaseResource(resH);
		}
		DmCloseDatabase(dbRef);
	}
	
	return versionP;
}

static Char *getLibVersionString()
{
	MemHandle	resH			= NULL;
	Char		*tverStringP	= NULL;
	Char		*versionP		= NULL;
	DmOpenRef	dbRef			= DmOpenDatabaseByTypeCreator('libr', 'CNLB', dmModeReadOnly);
	
	if (DmGetLastErr() == errNone)
	{
		resH = DmGet1Resource('tver', 1000);
		
		if (resH)
		{
			tverStringP = (Char *)MemHandleLock(resH);
			versionP = (Char *)MemPtrNew(StrLen(tverStringP) + 13);
			StrCopy(versionP, "拼音检索库: ");
			StrCat(versionP, tverStringP);
			MemHandleUnlock(resH);
			DmReleaseResource(resH);
		}
		DmCloseDatabase(dbRef);
	}
	
	return versionP;
}

static void customKey(KeyInfoType *cKey)
{
	FormType	*frmSetKeyP = NULL;
	EventType	event;
	Boolean		exit = false;
	
	//打开按键检测窗体
	FrmPopupForm(frmSetKey);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);

		if (event.eType != keyDownEvent)
		{
			if (! SysHandleEvent(&event))
			{
				switch (event.eType)
				{
					case frmLoadEvent:
					{
						frmSetKeyP = FrmInitForm(frmSetKey);
						FrmSetActiveForm(frmSetKeyP);
						break;
					}
					case frmOpenEvent:
					{
						FrmDrawForm(frmSetKeyP);
						break;
					}
					default:
					{
						if (frmSetKeyP)
						{
							FrmHandleEvent(frmSetKeyP, &event);
						}
						break;
					}
				}
			}
		}
		else
		{
			if (EvtKeydownIsVirtual(&event))
			{
				cKey->chr = event.data.keyDown.chr;
				cKey->keyCode = event.data.keyDown.keyCode;
				cKey->modifiers = event.data.keyDown.modifiers;
				exit = true;
			}
			else
			{
				FrmAlert(alertSetKeyError);
			}
		}
	}while (! exit);
	
	//清除按键事件
	MemSet(&event, sizeof(EventType), 0x00);
	event.eType = nilEvent;
	
	//退出窗体
	FrmReturnToForm(0);
}

static void drawPanel(FormType *frmMainP, UInt8 currentPanel, SettingType *settingP)
{
	RectangleType	r[3];
	UInt8			i;
	
	//画标签
	r[0].topLeft.x = 4; r[0].topLeft.y = 20; r[0].extent.x = 50; r[0].extent.y = 12;
	r[1].topLeft.x = 55; r[1].topLeft.y = 20; r[1].extent.x = 50; r[1].extent.y = 12;
	r[2].topLeft.x = 106; r[2].topLeft.y = 20; r[2].extent.x = 49; r[2].extent.y = 12;
	
	for (i = 0; i < 3; i ++)
	{
		if (i == currentPanel)
		{
			if (i != 2)
			{
				WinDrawLine(r[i].topLeft.x - 1, 16, r[i].topLeft.x + 50, 16);
				WinDrawLine(r[i].topLeft.x - 1, 16, r[i].topLeft.x - 1, 32);
				WinDrawLine(r[i].topLeft.x + 50, 16, r[i].topLeft.x + 50, 32);
				WinDrawLine(r[i].topLeft.x + 51, 18, r[i].topLeft.x + 51, 32);
			}
			else
			{
				WinDrawLine(r[i].topLeft.x - 1, 16, r[i].topLeft.x + 49, 16);
				WinDrawLine(r[i].topLeft.x - 1, 16, r[i].topLeft.x - 1, 32);
				WinDrawLine(r[i].topLeft.x + 49, 16, r[i].topLeft.x + 49, 32);
				WinDrawLine(r[i].topLeft.x + 50, 18, r[i].topLeft.x + 50, 32);
			}
		}
		else
		{
			WinDrawRectangleFrame(simpleFrame, &r[i]);
		}
	}
	WinDrawLine(106, 32, 156, 32);	//
	
	//画面板
	WinDrawLine(3, 32, 3, 125);
	WinDrawLine(156, 32, 156, 125);
	WinDrawLine(3, 125, 156, 125);
}

static void showPanel(	UInt8 thePanel,
						FormType *frmMainP,
						ModuleInfomationType *easyPhoneInfo,
						ModuleInfomationType *easyInputInfo,
						ModuleInfomationType *easyContactInfo,
						Char *easyPhoneVersionP,
						Char *easyInputVersionP,
						Char *easyContactVersionP,
						Char *libVersionP,
						SettingType *settingP)
{
	//显示及隐藏控件
	switch (thePanel)
	{
		case 0:
		{
			if (easyPhoneInfo->isInstalled)
			{
				//EasyPhone
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyPhone));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, cbAutoActiveInPhoneUI));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyPhoneDefaultField));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyPhoneDefaultField));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetPreFix));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneKey));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneBg));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, btnAdvanceSetting));
				
				//EasyInput
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyInput));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputPunc));
				
				//EasyContact
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyContact));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyContactModeTip));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyContactMode));
				
				FrmDrawForm(frmMainP);
				
				WinDrawChars(easyPhoneVersionP, StrLen(easyPhoneVersionP), 156 - FntCharsWidth(easyPhoneVersionP, StrLen(easyPhoneVersionP)), 33);
			}
			else
			{
				//EasyPhone
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyPhone));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbAutoActiveInPhoneUI));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetPreFix));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneBg));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnAdvanceSetting));
				
				//EasyInput
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyInput));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputPunc));
				
				//EasyContact
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyContact));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyContactModeTip));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyContactMode));
				
				FrmDrawForm(frmMainP);
				WinDrawChars("未安装随手Phone模块", 19, 10, 65);
			}
			break;
		}
		case 1:
		{
			if (easyInputInfo->isInstalled)
			{
				//EasyPhone
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyPhone));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbAutoActiveInPhoneUI));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetPreFix));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneBg));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnAdvanceSetting));
				
				//EasyInput
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyInput));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputKey));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputPunc));
				
				//EasyContact
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyContact));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyContactModeTip));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyContactMode));
				
				FrmDrawForm(frmMainP);
				
				WinDrawChars(easyInputVersionP, StrLen(easyInputVersionP), 156 - FntCharsWidth(easyInputVersionP, StrLen(easyInputVersionP)), 33);
			}
			else if (frmMainP)
			{
				//EasyPhone
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyPhone));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbAutoActiveInPhoneUI));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetPreFix));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneBg));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnAdvanceSetting));
				
				//EasyInput
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyInput));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputPunc));
				
				//EasyContact
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyContact));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyContactModeTip));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyContactMode));
				
				FrmDrawForm(frmMainP);
				WinDrawChars("未安装输入易模块", 16, 10, 65);
			}
			break;
		}
		case 2:
		{
			if (easyContactInfo->isInstalled)
			{
				//EasyPhone
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyPhone));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbAutoActiveInPhoneUI));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetPreFix));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneBg));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnAdvanceSetting));
				
				//EasyInput
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyInput));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputPunc));
				
				//EasyContact
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyContact));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyContactModeTip));
				FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyContactMode));
				
				FrmDrawForm(frmMainP);
				
				WinDrawChars(easyContactVersionP, StrLen(easyContactVersionP), 156 - FntCharsWidth(easyContactVersionP, StrLen(easyContactVersionP)), 33);
			}
			else if (frmMainP)
			{
				//EasyPhone
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyPhone));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbAutoActiveInPhoneUI));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyPhoneDefaultField));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetPreFix));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyPhoneBg));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnAdvanceSetting));
				
				//EasyInput
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyInput));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputKey));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnSetEasyInputPunc));
				
				//EasyContact
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyContact));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, lbEasyContactModeTip));
				FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, ptriEasyContactMode));
				
				FrmDrawForm(frmMainP);
				WinDrawChars("未安装通讯录检索模块", 20, 10, 65);
			}
			break;
		}
	}
	
	if (libVersionP)
	{
		WinDrawChars(libVersionP, StrLen(libVersionP), 2, 150);
	}
	else
	{
		WinDrawChars("未安装拼音检索库！", 18, 2, 148);
	}
}

static void frmMainEventHandler(Boolean isLaunchFromPanel)
{
	SettingType				setting;
	UInt16					period;
	UInt16					doubleTapDelay;
	Boolean					bQueueAhead;
	Boolean					exitToPref = false;
	UInt32					companyID;
	UInt32					deviceID;
	ModuleInfomationType	easyPhoneInfo;
	ModuleInfomationType	easyInputInfo;
	ModuleInfomationType	easyContactInfo;
	EventType				event;
	UInt16					error;
	SettingType				*settingP;
	UInt32					settingAddress;
	UInt16					cardNo;
	LocalID					dbId;
	FormType				*frmMainP					= NULL;
	Boolean					frmMainDrawed				= false;
	ListType				*lstEasyInputKeyP			= NULL;
	ListType				*lstEasyContactModeP		= NULL;
	ListType				*lstEasyPhoneDefaultFieldP	= NULL;
	UInt8					currentPanel				= 0;
	Boolean					isMenuNotActive				= true;
	UInt16					settingSize					= sizeof(SettingType);
	Char					*easyPhoneVersionP			= NULL;
	Char					*easyInputVersionP			= NULL;
	Char					*easyContactVersionP		= NULL;
	Char					*libVersionP				= getLibVersionString();
	
	//获取设置信息
	if (PrefGetAppPreferences(APPLICATION_CREATOR, APPLICATION_PREF_ID, &setting, &settingSize, true) == noPreferenceFound)
	{ //第一次运行
		MemSet(&setting, sizeof(SettingType), 0x00);
		
		//公共设置
		//检测Treo型号
		FtrGet(sysFtrCreator, sysFtrNumOEMCompanyID, &companyID);
		FtrGet(sysFtrCreator, sysFtrNumOEMDeviceID, &deviceID);
		
		
		if ((companyID == 'Palm' || companyID == 'hspr') && (deviceID == 'D053')) //Treo680
		{
			setting.treoModel = TREO_680;
			
			setting.phoneFormId = PHONE_FORM_ID_TREO680;
			setting.phoneFormObjectNumber = PHONE_FORM_OBJECT_NUMBER_TREO680;
			setting.smsFormId = SMS_FORM_ID_TREO680;
			setting.smsFormObjectNumber = SMS_FORM_OBJECT_NUMBER_TREO680;
			setting.smsFormFieldIndex = SMS_FORM_FIELD_INDEX_TREO680;
			setting.contactFormId = CONTACT_FORM_ID_TREO680;
			setting.contactFormObjectNumber = CONTACT_FORM_OBJECT_NUMBER_TREO680;
			setting.contactFormTableIndex = CONTACT_FORM_TABLE_INDEX_TREO680;
		}
		else if ((companyID == 'Palm' || companyID == 'hspr') && (deviceID == 'D061'||deviceID == 'D062')) //Centro by Bob
		{
			setting.treoModel = CENTRO;
			
			setting.phoneFormId = PHONE_FORM_ID_CENTRO;
			setting.phoneFormObjectNumber = PHONE_FORM_OBJECT_NUMBER_CENTRO;
			setting.smsFormId = SMS_FORM_ID_CENTRO;
			setting.smsFormObjectNumber = SMS_FORM_OBJECT_NUMBER_CENTRO;
			setting.smsFormFieldIndex = SMS_FORM_FIELD_INDEX_CENTRO;
			setting.contactFormId = CONTACT_FORM_ID_CENTRO;
			setting.contactFormObjectNumber = CONTACT_FORM_OBJECT_NUMBER_CENTRO;
			setting.contactFormTableIndex = CONTACT_FORM_TABLE_INDEX_CENTRO;
		}
		else if ((companyID == 'Palm' || companyID == 'hspr') && deviceID == 'D052') //Treo700P by Bob
		{
			setting.treoModel = TREO_680;
			
			setting.phoneFormId = PHONE_FORM_ID_TREO650;
			setting.phoneFormObjectNumber = PHONE_FORM_OBJECT_NUMBER_TREO650;
			setting.smsFormId = SMS_FORM_ID_TREO680;
			setting.smsFormObjectNumber = SMS_FORM_OBJECT_NUMBER_TREO680;
			setting.smsFormFieldIndex = SMS_FORM_FIELD_INDEX_TREO680;
			setting.contactFormId = CONTACT_FORM_ID_TREO650;
			setting.contactFormObjectNumber = CONTACT_FORM_OBJECT_NUMBER_TREO650;
			setting.contactFormTableIndex = CONTACT_FORM_TABLE_INDEX_TREO650;
		}
		else if (companyID == 'hspr' && (deviceID == 'H102' || deviceID == 'H202')) //Treo650
		{
			setting.treoModel = TREO_650;
			
			setting.phoneFormId = PHONE_FORM_ID_TREO650;
			setting.phoneFormObjectNumber = PHONE_FORM_OBJECT_NUMBER_TREO650;
			setting.smsFormId = SMS_FORM_ID_TREO650;
			setting.smsFormObjectNumber = SMS_FORM_OBJECT_NUMBER_TREO650;
			setting.smsFormFieldIndex = SMS_FORM_FIELD_INDEX_TREO650;
			setting.contactFormId = CONTACT_FORM_ID_TREO650;
			setting.contactFormObjectNumber = CONTACT_FORM_OBJECT_NUMBER_TREO650;
			setting.contactFormTableIndex = CONTACT_FORM_TABLE_INDEX_TREO650;
		}
		else if (companyID == 'hspr' && (deviceID == 'H101' || deviceID == 'H201')) //Treo600
		{
			setting.treoModel = TREO_600;
			
			setting.phoneFormId = PHONE_FORM_ID_TREO600;
			setting.phoneFormObjectNumber = PHONE_FORM_OBJECT_NUMBER_TREO600;
			setting.smsFormId = SMS_FORM_ID_TREO600;
			setting.smsFormObjectNumber = SMS_FORM_OBJECT_NUMBER_TREO600;
			setting.smsFormFieldIndex = SMS_FORM_FIELD_INDEX_TREO600;
			setting.contactFormId = CONTACT_FORM_ID_TREO600;
			setting.contactFormObjectNumber = CONTACT_FORM_OBJECT_NUMBER_TREO600;
			setting.contactFormTableIndex = CONTACT_FORM_TABLE_INDEX_TREO600;
		}
		else
		{
			setting.treoModel = NOT_A_TREO;
			
			setting.phoneFormId = 0;
			setting.phoneFormObjectNumber = 0;
			setting.smsFormId = 0;
			setting.smsFormObjectNumber = 0;
			setting.smsFormFieldIndex = 0;
			setting.contactFormId = 0;
			setting.contactFormObjectNumber = 0;
			setting.contactFormTableIndex = 0;
		}
		setting.orderlySearch = false;
		
		//EasyPhone设置
		StrCopy(setting.preFix[0], "-");
		StrCopy(setting.preFix[1], "17951");
		StrCopy(setting.preFix[2], "17911");
		StrCopy(setting.preFix[3], "106");
		setting.customActiveKey.chr = keyHard2;
		setting.customActiveKey.keyCode = keyHard2;
		setting.customActiveKey.modifiers = (willSendUpKeyMask | commandKeyMask);
		KeyRates(false, &setting.initDelay, &period, &doubleTapDelay, &bQueueAhead);
		setting.isPhoneUIEnabled = true;
		setting.defaultField = 0xFF;
		setting.skin.frmContacts.bounds.topLeft.x = 5;
		setting.skin.frmContacts.bounds.topLeft.y = 7;
		setting.skin.frmContacts.bounds.extent.x = 150;
		setting.skin.frmContacts.bounds.extent.y = 149;
		setting.skin.frmContacts.other = 0;
		//5,4,150,90,8
		setting.skin.lstContacts.bounds.topLeft.x = 10;
		setting.skin.lstContacts.bounds.topLeft.y = 11;
		setting.skin.lstContacts.bounds.extent.x = 140;
		setting.skin.lstContacts.bounds.extent.y = 90;
		setting.skin.lstContacts.other = 8;
		//2,103,20,22,2
		setting.skin.gdtDialButton.bounds.topLeft.x = 8;
		setting.skin.gdtDialButton.bounds.topLeft.y = 113;
		setting.skin.gdtDialButton.bounds.extent.x = 20;
		setting.skin.gdtDialButton.bounds.extent.y = 25;
		setting.skin.gdtDialButton.other = 2;
		//25,107,131,16,0
		setting.skin.fldPhoneNumber.bounds.topLeft.x = 30;
		setting.skin.fldPhoneNumber.bounds.topLeft.y = 113;
		setting.skin.fldPhoneNumber.bounds.extent.x = 121;
		setting.skin.fldPhoneNumber.bounds.extent.y = 25;
		setting.skin.fldPhoneNumber.other = 0;
		//2,126,78,10,2
		setting.skin.gdtPreFixTrigger.bounds.topLeft.x = 8;
		setting.skin.gdtPreFixTrigger.bounds.topLeft.y = 140;
		setting.skin.gdtPreFixTrigger.bounds.extent.x = 66;
		setting.skin.gdtPreFixTrigger.bounds.extent.y = 12;
		setting.skin.gdtPreFixTrigger.other = 2;
		//81,126,29,10,2
		setting.skin.gdtOperationButton.bounds.topLeft.x = 75;
		setting.skin.gdtOperationButton.bounds.topLeft.y = 140;
		setting.skin.gdtOperationButton.bounds.extent.x = 29;
		setting.skin.gdtOperationButton.bounds.extent.y = 12;
		setting.skin.gdtOperationButton.other = 2;
		//111,126,29,10,2
		setting.skin.gdtSMSButton.bounds.topLeft.x = 105;
		setting.skin.gdtSMSButton.bounds.topLeft.y = 140;
		setting.skin.gdtSMSButton.bounds.extent.x = 29;
		setting.skin.gdtSMSButton.bounds.extent.y = 12;
		setting.skin.gdtSMSButton.other = 2;
		//141,126,16,10,2
		setting.skin.gdtHelpButton.bounds.topLeft.x = 135;
		setting.skin.gdtHelpButton.bounds.topLeft.y = 140;
		setting.skin.gdtHelpButton.bounds.extent.x = 16;
		setting.skin.gdtHelpButton.bounds.extent.y = 12;
		setting.skin.gdtHelpButton.other = 2;
		
		setting.showDetailType = SHOW_DETAIL;
		setting.keyHard3Action = KEY_HARD3_EASYPHONE_MESSAGE;

		//EasyInput设置
		setting.easyInputActiveKey = keyHard3;
		StrCopy(setting.punc[0], "～");
		StrCopy(setting.punc[1], "^_^");
		StrCopy(setting.punc[2], "^_-");
		StrCopy(setting.punc[3], ">_<");
		StrCopy(setting.punc[4], "-_-!!");
		StrCopy(setting.punc[5], "-_-#");
		StrCopy(setting.punc[6], "@_@");
		StrCopy(setting.punc[7], "ToT");
		
		//EasyContact设置
		setting.searchMode = LASTNAME_MODE;
		
		PrefSetAppPreferences(APPLICATION_CREATOR, APPLICATION_PREF_ID, APPLICATION_PREF_VER, &setting, settingSize, true);
	}
	
	//检测已经安装的模块
	getInstalledModule(&easyPhoneInfo, &easyInputInfo, &easyContactInfo);
	if (! easyPhoneInfo.isInstalled)
	{
		setting.isEasyPhoneEnabled = false;
	}
	else
	{
		easyPhoneVersionP = getVersionString(easyPhoneInfo.cardNo, easyPhoneInfo.dbId);
	}
	if (! easyInputInfo.isInstalled)
	{
		setting.isEasyInputEnabled = false;
	}
	else
	{
		easyInputVersionP = getVersionString(easyInputInfo.cardNo, easyInputInfo.dbId);
	}
	if (! easyContactInfo.isInstalled)
	{
		setting.isEasyContactEnabled = false;
	}
	else
	{
		easyContactVersionP = getVersionString(easyContactInfo.cardNo, easyContactInfo.dbId);
	}
	
	//取消注册按键消息
	SysCurAppDatabase(&cardNo, &dbId);
	SysNotifyUnregister(cardNo, dbId, sysNotifyDBDeletedEvent, sysNotifyNormalPriority);
	SysNotifyUnregister(cardNo, dbId, sysNotifyEventDequeuedEvent, sysNotifyNormalPriority);
	
	//如果已经被启动过，清除之前的setting指针
	if (FtrGet(APPLICATION_CREATOR, 0, &settingAddress) == errNone)
	{
		settingP = (SettingType *)settingAddress;
		
		MemPtrFree(settingP);
		FtrUnregister(APPLICATION_CREATOR, 0);
	}
	//若有载入的图片，清除
	if (FtrGet('EzPH', 1, &settingAddress) == errNone)
	{
		FtrPtrFree('EzPH', 1);
	}
	setting.skin.bmpBitsP = NULL;
	setting.skin.bmpWidth = 0;
	setting.skin.bmpHeight = 0;
	setting.skin.bmpDensity = kDensityLow;
	setting.skin.bmpBitsP = NULL;
	
	//打开窗口
	FrmGotoForm(frmMain);
	
	do
	{
		EvtGetEvent(&event, evtWaitForever);
		
		if (! SysHandleEvent(&event))
		{
			if (! MenuHandleEvent(0, &event, &error))
			{
				switch (event.eType)
				{
					case frmLoadEvent: //装载窗体
					{
						frmMainP = FrmInitForm(frmMain);
						FrmSetActiveForm(frmMainP);
						
						break;
					}
					case frmOpenEvent: //打开窗体
					{
						//获取控件指针
						lstEasyContactModeP = getObjectPtr(frmMainP, lstEasyContactMode);
						lstEasyPhoneDefaultFieldP = getObjectPtr(frmMainP, lstEasyPhoneDefaultField);
						
						//显示设置
						FrmSetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyPhone), setting.isEasyPhoneEnabled);
						FrmSetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyInput), setting.isEasyInputEnabled);
						FrmSetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyContact), setting.isEasyContactEnabled);
						FrmSetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbAutoActiveInPhoneUI), setting.isNotAutoLaunchInPhoneUI);
						FrmSetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbOrderlySearch), setting.orderlySearch);
						
						LstSetSelection(lstEasyContactModeP, setting.searchMode);
						CtlSetLabel(getObjectPtr(frmMainP, ptriEasyContactMode), LstGetSelectionText(lstEasyContactModeP, LstGetSelection(lstEasyContactModeP)));
						
						if (setting.defaultField == 0xFF)
						{
							LstSetSelection(lstEasyPhoneDefaultFieldP, 0);
						}
						else
						{
							LstSetSelection(lstEasyPhoneDefaultFieldP, setting.defaultField + 1);
						}
						CtlSetLabel(getObjectPtr(frmMainP, ptriEasyPhoneDefaultField), LstGetSelectionText(lstEasyPhoneDefaultFieldP, LstGetSelection(lstEasyPhoneDefaultFieldP)));
						
						if (setting.isShowInPref)
						{
							FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, btnBackToPref));
						}
						else
						{
							FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnBackToPref));
						}
						showPanel(currentPanel, frmMainP, &easyPhoneInfo, &easyInputInfo, &easyContactInfo, easyPhoneVersionP, easyInputVersionP, easyContactVersionP, libVersionP, &setting);
						drawPanel(frmMainP, currentPanel, &setting);
						if (setting.treoModel == TREO_600)
						{
							WinDrawChars("Treo 600", 8, 120, 1);
						}
						else if (setting.treoModel == TREO_650)
						{
							WinDrawChars("Treo 650", 8, 120, 1);
						}
						else if (setting.treoModel == TREO_680)
						{
							WinDrawChars("Treo 680", 8, 120, 1);
						}
						else if (setting.treoModel == CENTRO)
						{
							WinDrawChars("  Centro", 8, 120, 1);
						}
						else
						{
							WinDrawChars("未知型号", 8, 120, 1);
						}
						frmMainDrawed = true;
						
						FrmSetNavState(frmMainP, kFrmNavStateFlagsObjectFocusMode);
						
						break;
					}
					case frmUpdateEvent:
					{
						HsNavRemoveFocusRing(frmMainP);
						
						showPanel(currentPanel, frmMainP, &easyPhoneInfo, &easyInputInfo, &easyContactInfo, easyPhoneVersionP, easyInputVersionP, easyContactVersionP, libVersionP, &setting);
						drawPanel(frmMainP, currentPanel, &setting);
						if (setting.treoModel == TREO_600)
						{
							WinDrawChars("Treo 600", 8, 120, 1);
						}
						else if (setting.treoModel == TREO_650)
						{
							WinDrawChars("Treo 650", 8, 120, 1);
						}
						else if (setting.treoModel == TREO_680)
						{
							WinDrawChars("Treo 680", 8, 120, 1);
						}
						else if (setting.treoModel == CENTRO)
						{
							WinDrawChars("  Centro", 8, 120, 1);
						}
						else
						{
							WinDrawChars("未知型号", 8, 120, 1);
						}
						break;
					}
					case frmObjectFocusTakeEvent:
					{
						FrmDispatchEvent(&event);
						drawPanel(frmMainP, currentPanel, &setting);
						break;
					}
					case winEnterEvent: //是否打开或退出菜单
					{
						if (frmMainP)
						{
							if (event.data.winEnter.enterWindow == (WinHandle)frmMainP)
							{
								isMenuNotActive = true;
							}
							else
							{
								isMenuNotActive = false;
							}
						}
						break;
					}
					case menuEvent:
					{
						isMenuNotActive = true;
						switch (event.data.menu.itemID)
						{
							case menuitemAbout:
							{
								FormType	*frmAboutP = FrmInitForm(frmAbout);
								
								FrmDoDialog(frmAboutP);
								
								FrmDeleteForm(frmAboutP);
								break;
							}
							case menuitemEasyPhoneHelp:
							{
								FrmHelp(stringEasyPhoneHelp);
								break;
							}
							case menuitemEasyInputHelp:
							{
								FrmHelp(stringEasyInputHelp);
								break;
							}
							case menuitemEasyContactHelp:
							{
								FrmHelp(stringEasyContactHelp);
								break;
							}
							case menuitemSettingHelp:
							{
								FrmHelp(stringSettingHelp);
								break;
							}
							case menuitemShowInLauncher:
							{
								UInt32	db_type = 'appl';
								
								FrmHideObject(frmMainP, FrmGetObjectIndex(frmMainP, btnBackToPref));
								setting.isShowInPref = false;
								SysCurAppDatabase(&cardNo, &dbId);
								DmSetDatabaseInfo(cardNo, dbId, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &db_type, NULL);
								break;
							}
							case menuitemShowInPref:
							{
								UInt32	db_type = 'panl';
								
								FrmShowObject(frmMainP, FrmGetObjectIndex(frmMainP, btnBackToPref));
								setting.isShowInPref = true;
								SysCurAppDatabase(&cardNo, &dbId);
								DmSetDatabaseInfo(cardNo, dbId, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &db_type, NULL);
								break;
							}
						}
						
						FrmUpdateForm(frmMain, frmRedrawUpdateCode);
						break;
					}
					case ctlSelectEvent:
					{
						HsNavRemoveFocusRing(frmMainP);
						switch (event.data.ctlSelect.controlID)
						{
							case btnEasyPhonePanel:
							{
								currentPanel = 0;
								
								break;
							}
							case btnEasyInputPanel:
							{
								currentPanel = 1;
								
								break;
							}
							case btnEasyContactPanel:
							{
								currentPanel = 2;
								
								break;
							}
							case btnSetPreFix:
							{
								setPreFix(&setting);
								break;
							}
							case btnSetEasyPhoneKey:
							{
								customKey(&setting.customActiveKey);
								break;
							}
							case btnSetEasyPhoneBg:
							{
								setSkin(&setting);
								break;
							}
							case btnSetEasyInputPunc:
							{
								setPunc(&setting);
								break;
							}
							case btnSetEasyInputKey:
							{
								KeyInfoType		eipKey;
								
								//eipKey.keyCode = setting.easyInputActiveKey;
								customKey(&eipKey);
								setting.easyInputActiveKey = eipKey.keyCode;
								break;
							}
							case btnAdvanceSetting:
							{
								setAdvanceSetting(&setting);
								break;
							}
							case btnBackToPref:
							{
								exitToPref = true;
								break;
							}
						}
						
						FrmUpdateForm(frmMain, frmRedrawUpdateCode);
						HsNavObjectTakeFocus(frmMainP, event.data.ctlSelect.controlID);
					}
					default:
					{
						//FrmHandleEvent(frmMainP, &event);
						FrmDispatchEvent(&event);
						
						break;
					}
				}
			}
		}
	} while (event.eType != appStopEvent && (! exitToPref));
	
	//清除版本信息
	if (easyPhoneVersionP)
	{
		MemPtrFree(easyPhoneVersionP);
	}
	if (easyInputVersionP)
	{
		MemPtrFree(easyInputVersionP);
	}
	if (easyContactVersionP)
	{
		MemPtrFree(easyContactVersionP);
	}
	if (libVersionP)
	{
		MemPtrFree(libVersionP);
	}
	
	//保存参数
	setting.isEasyPhoneEnabled = (Boolean)FrmGetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyPhone));
	setting.isEasyInputEnabled = (Boolean)FrmGetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyInput));
	setting.isEasyContactEnabled = (Boolean)FrmGetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbEnableEasyContact));
	setting.orderlySearch = (Boolean)FrmGetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbOrderlySearch));
	setting.isNotAutoLaunchInPhoneUI = (Boolean)FrmGetControlValue(frmMainP, FrmGetObjectIndex(frmMainP, cbAutoActiveInPhoneUI));
	if (LstGetSelection(lstEasyPhoneDefaultFieldP) == noListSelection ||
		LstGetSelection(lstEasyPhoneDefaultFieldP) == 0)
	{
		setting.defaultField = 0xFF;
	}
	else
	{
		setting.defaultField = LstGetSelection(lstEasyPhoneDefaultFieldP) - 1;
	}
	setting.searchMode = LstGetSelection(lstEasyContactModeP);
	PrefSetAppPreferences(APPLICATION_CREATOR, APPLICATION_PREF_ID, APPLICATION_PREF_VER, &setting, settingSize, true);
	
	if (setting.isEasyPhoneEnabled || setting.isEasyInputEnabled || setting.isEasyContactEnabled)
	{
		settingP = (SettingType *)MemPtrNew(sizeof(SettingType));
		
		if (settingP)
		{
			settingAddress = (UInt32)settingP;
			FtrSet(APPLICATION_CREATOR, 0, settingAddress);
			
			MemMove(settingP, &setting, sizeof(SettingType));
			settingP->currentField = NULL;
			MemPtrSetOwner(settingP, 0);
			
			if (setting.isEasyPhoneEnabled)
			{
				if ((UInt8)setting.skin.bmpPath[0] != 0)
				{
					if (loadSkinToFeature(settingP) != errNone)
					{
						FrmAlert(alertFileError);
						setting.isEasyPhoneEnabled = false;
					}
				}
			}
			else
			{
				settingP->skin.bmpWidth = 0;
				settingP->skin.bmpHeight = 0;
				settingP->skin.bmpDensity = kDensityLow;
				settingP->skin.bmpBitsP = NULL;
			}
			
			if (setting.isEasyPhoneEnabled || setting.isEasyInputEnabled || setting.isEasyContactEnabled)
			{
				SysCurAppDatabase(&cardNo, &dbId);
				SysNotifyRegister(cardNo, dbId, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, settingP);
				SysNotifyRegister(cardNo, dbId, sysNotifyDBDeletedEvent, NULL, sysNotifyNormalPriority, settingP);
			}
		}
		else
		{
			FrmAlert(alertMemoryError);
		}
	}
	
	FrmCloseAllForms();
	
	if (exitToPref)
	{
		LaunchWithCommand('appl', 'pref', sysAppLaunchCmdNormalLaunch, NULL);
	}
}

static UInt8 isInActableStatus(SettingType *settingP, WChar chr, UInt16 keyCode, UInt16 modifiers)
{
	UInt16		currentFormId = FrmGetActiveFormID();
	
	if ((! settingP->isNotAutoLaunchInPhoneUI) && currentFormId == settingP->phoneFormId)
	{
		FormType	*currentForm = FrmGetActiveForm();
		
		//控件数是否匹配
		if (FrmGetNumberOfObjects(currentForm) == settingP->phoneFormObjectNumber)
		{
			//是否有其他Window，如菜单在窗体之上
			if (FrmGetWindowHandle(currentForm) == WinGetActiveWindow())
			{
				//画状态字符
				if (! settingP->isTempDisabled)
				{
					WinDrawChars("中", 2, 111, 1);
				}
				
				//判断按键
				if ((chr >= keyA && chr <=keyZ) || (chr >= keyZero && chr <= keyNine) || chr == keyReturn || (keyCode == hsKeySymbol && settingP->treoModel == TREO_600))
				{
					if ((! settingP->isTempDisabled) || chr == keyReturn)
					{
						if (! (modifiers & willSendUpKeyMask))
						{
							//返回在电话界面
							return IN_PHONE_FORM;
						}
						else
						{
							return EVENT_SHOULD_BE_CLEAR;
						}
					}
				}
			}
		}
	}
	else if (settingP->isEasyContactEnabled && currentFormId == settingP->contactFormId && modifiers > 0x0000)
	{
		FormType	*currentForm = FrmGetActiveForm();
		
		//控件数是否匹配
		if (FrmGetNumberOfObjects(currentForm) == settingP->contactFormObjectNumber)
		{
			//检查特定控件的类型是否匹配
			if (FrmGetObjectType(currentForm, settingP->contactFormTableIndex) == frmTableObj)
			{
				//是否有其他Window，如菜单在窗体之上
				if (FrmGetWindowHandle(currentForm) == WinGetActiveWindow())
				{
					//判断按键
					if (chr >= keyA && chr <=keyZ)
					{
						if (! (modifiers & willSendUpKeyMask))
						{
							//返回在通讯录界面
							return IN_CONTACT_FORM;
						}
						else
						{
							return EVENT_SHOULD_BE_CLEAR;
						}
					}
				}
			}
		}
	}
	else if (settingP->isEasyInputEnabled)
	{
		if (keyCode == settingP->easyInputActiveKey)
		{
			if (InsPtEnabled())
			{
				FormType	*currentForm = FrmGetActiveForm();
				
				//是否有其他Window，如菜单在窗体之上
				if (FrmGetWindowHandle(currentForm) == WinGetActiveWindow())
				{
					if (FrmGetFocus(currentForm) != noFocus)
					{
						if (modifiers & willSendUpKeyMask)
						{
							return IN_FOCUSED_FIELD;
						}
						else
						{
							return EVENT_SHOULD_BE_CLEAR;
						}
					}
				}
			}
		}
		else if (keyCode == vchrRockerCenter)
		{
			UInt16 currentFormId = FrmGetActiveFormID();
			
			if (currentFormId == settingP->smsFormId)
			{
				FormType	*currentForm = FrmGetActiveForm();
				/*
				//检测当前界面上的objects数 by Bob
				Int32 num = FrmGetNumberOfObjects(currentForm);
				Char string;
				StrIToA (&string,num);
				ClipboardAddItem(clipboardText,&string, StrLen(&string));
				*/
				
				//控件数是否匹配
				if (FrmGetNumberOfObjects(currentForm) == settingP->smsFormObjectNumber)
				{
					//检查获得焦点的控件是否匹配
					if (FrmGetFocus(currentForm) == settingP->smsFormFieldIndex)
					{
						//返回在短讯界面的联系人文本框
						settingP->currentField = (FieldType *)FrmGetObjectPtr(currentForm, settingP->smsFormFieldIndex);
						
						if (settingP->currentField)
						{
							if (FldGetTextLength(settingP->currentField) == 0)
							{
								if (! (modifiers & willSendUpKeyMask))
								{
									return IN_SMS_FORM_FIELD;
								}
								else
								{
									return EVENT_SHOULD_BE_CLEAR;
								}
							}
						}
					}
				}
			}
		}
	}
	
	return INVAILD_FORM;
}

static WChar keyTranslator(WChar chr)
{
	switch (chr)
	{
		case keyE:
		{
			return keyOne;
			break;
		}
		case keyR:
		{
			return keyTwo;
			break;
		}
		case keyT:
		{
			return keyThree;
			break;
		}
		case keyD:
		{
			return keyFour;
			break;
		}
		case keyF:
		{
			return keyFive;
			break;
		}
		case keyG:
		{
			return keySix;
			break;
		}
		case keyX:
		{
			return keySeven;
			break;
		}
		case keyC:
		{
			return keyEight;
			break;
		}
		case keyV:
		{
			return keyNine;
			break;
		}
	}
	
	return chr;
}

static Boolean isEventShouldBePost(EventType *eventP, SettingType *settingP, Boolean *isNotifyHandled)
{
	WChar		chr					= byteSwap16(eventP->data.keyDown.chr);
	UInt16		keyCode				= byteSwap16(eventP->data.keyDown.keyCode);
	UInt16		modifiers			= byteSwap16(eventP->data.keyDown.modifiers);
	
	//是否在合适的窗体？
	switch (isInActableStatus(settingP, chr, keyCode, modifiers))
	{
		case INVAILD_FORM:
		{
			if (settingP->isEasyPhoneEnabled)
			{
				UInt16		aModifiers			= settingP->customActiveKey.modifiers;
				UInt16		staticModifiers		= STATIC_MODIFIERS;
				Boolean		bWaitForKeyRelease	= false;
				UInt32		startTicks			= TimGetTicks();
				
				//检查是否长按
				modifiers &= (~poweredOnKeyMask);
				
				if (chr == settingP->customActiveKey.chr &&
					keyCode == settingP->customActiveKey.keyCode &&
					(modifiers & staticModifiers) == aModifiers)
				{
					bWaitForKeyRelease = true;
				}
				
				// wait for key to be released
				if (bWaitForKeyRelease)
				{
					Boolean bKeepGoing = true;
					UInt32  repeatTime = startTicks + settingP->initDelay;
					
					while(KeyCurrentState() && bKeepGoing)
					{
						if(TimGetTicks() >= repeatTime)
						{
							bKeepGoing = false;
							modifiers |= autoRepeatKeyMask;
							eventP->data.keyDown.chr = NULL;
							*isNotifyHandled = true;
						}
					}
					
					aModifiers |= autoRepeatKeyMask;
				}
				
				// set static modifiers
				staticModifiers = STATIC_MODIFIERS | DYNAMIC_MODIFIERS;
				
				if (chr == settingP->customActiveKey.chr &&
					keyCode == settingP->customActiveKey.keyCode &&
					(modifiers & staticModifiers) == aModifiers)
				{
					*isNotifyHandled = true;
					settingP->actionCode = EASY_PHONE_LONG_PRESS_ACTIVE;
					settingP->actionCreator = 'EzPH';
					settingP->actionKey = chrNull;
					settingP->actionKeycode = chrNull;
					return true;
				}
			}
			
			break;
		}
		case IN_PHONE_FORM:
		{
			//在电话界面
			if (chr == keyReturn)
			{
				settingP->isTempDisabled = (! settingP->isTempDisabled);
				if (! settingP->isTempDisabled)
				{
					WinDrawChars("中", 2, 111, 1);
				}
				else
				{
					WinEraseChars("中", 2, 111, 1);
				}
				*isNotifyHandled = true;
			}
			else
			{
				settingP->actionCode = EASY_PHONE_PHONE_UI_ACTIVE;
				settingP->actionCreator = 'EzPH';
				if (settingP->treoModel == TREO_600 && keyCode == hsKeySymbol)
				{
					settingP->actionKey = keyZero;
				}
				else
				{
					settingP->actionKey = keyTranslator(chr);
				}
				settingP->actionKeycode = keyCode;
				return true;
			}
			
			break;
		}
		case IN_FOCUSED_FIELD:
		{
			//在普通文本框
			settingP->actionCode = EASY_INPUT_KEY_ACTIVE;
			settingP->actionCreator = 'EzIP';
			settingP->actionKey = chrNull;
			settingP->actionKeycode = chrNull;
			return true;
			
			break;
		}
		case IN_SMS_FORM_FIELD:
		{
			//在短讯界面的联系人文本框
			settingP->actionCode = EASY_INPUT_SMS_FIELD_ACTIVE;
			settingP->actionCreator = 'EzIP';
			settingP->actionKey = chrNull;
			settingP->actionKeycode = chrNull;
			return true;
			
			break;
		}
		case IN_CONTACT_FORM:
		{
			//在通讯录界面
			settingP->actionCode = EASY_CONTACT_ACTIVE;
			settingP->actionCreator = 'EzCT';
			if (settingP->treoModel == TREO_600 && keyCode == hsKeySymbol)
			{
				settingP->actionKey = keyZero;
			}
			else
			{
				settingP->actionKey = chr;
			}
			settingP->actionKeycode = keyCode;
			return true;
			
			break;
		}
		case EVENT_SHOULD_BE_CLEAR:
		{
			*isNotifyHandled = true;
			
			break;
		}
	}
	
	return false;
}

#if 0
#pragma mark -
#pragma mark Functions for Easyphone Action
#pragma mark -
#endif


static void dialPhoneNumber(ActionParameterType *actionParameterP)
{
	SysNotifyParamType		*param;
	HelperNotifyEventType	*details;
	HelperNotifyExecuteType	*execute;

	//初始化参数
	param = MemPtrNew(sizeof(SysNotifyParamType));
	details = MemPtrNew(sizeof(HelperNotifyEventType));
	execute = MemPtrNew(sizeof(HelperNotifyExecuteType));
	
	param->notifyType = sysNotifyHelperEvent;
	param->broadcaster = APPLICATION_CREATOR;
	param->notifyDetailsP = details;
	param->handled = false;

	details->version = kHelperNotifyCurrentVersion;
	details->actionCode = kHelperNotifyActionCodeExecute;
	details->data.executeP = execute;

	execute->serviceClassID = kHelperServiceClassIDVoiceDial;
	execute->helperAppID = 0;
	execute->dataP = (Char *)MemPtrNew(StrLen(actionParameterP->phoneNumber) + 1);
	StrCopy(execute->dataP, actionParameterP->phoneNumber);
	execute->displayedName = (Char *)MemPtrNew(StrLen(actionParameterP->fullName) + 1);
	StrCopy(execute->displayedName, actionParameterP->fullName);
	execute->detailsP = 0;
	execute->err = errNone;
	
	MemPtrSetOwner(execute->dataP, 0);
	MemPtrSetOwner(execute->displayedName, 0);
	MemPtrSetOwner(execute, 0);
	MemPtrSetOwner(details, 0);
	MemPtrSetOwner(param, 0);
	
	MemPtrFree(actionParameterP);
	
	SysNotifyBroadcast(param);
}

static void performAction(ActionParameterType *actionParamP, SettingType *settingP)
{
	/*
		#define EXIT_FOR_DIAL			0x02
		#define EXIT_FOR_SYSTEM_MESSAGE	0x04
		#define EXIT_FOR_EMAIL			0x05
		#define EXIT_FOR_ADD_CONTACT	0x06
		#define EXIT_FOR_OPEN_CONTACT	0x07
		#define EXIT_FOR_DIRECT_DIAL	0x09
	*/
	Char	*phoneNumberP;
	Char	*nameP;
	
	switch (actionParamP->actionCode)
	{
		case EXIT_FOR_DIAL:			//拨打电话
		case EXIT_FOR_DIRECT_DIAL:
		{
			dialPhoneNumber(actionParamP);
			break;
		}
		case EXIT_FOR_SYSTEM_MESSAGE:	//发送短讯
		{
			//构造参数
			phoneNumberP = (Char *)MemPtrNew(StrLen(actionParamP->phoneNumber) + 1);
			nameP = (Char *)MemPtrNew(StrLen(actionParamP->fullName) + 1);
			StrCopy(phoneNumberP, actionParamP->phoneNumber);
			MemPtrSetOwner(phoneNumberP, 0);
			StrCopy(nameP, actionParamP->fullName);
			MemPtrSetOwner(nameP, 0);
			
			//释放内存
			MemPtrFree(actionParamP);
			
			//调用短讯程序
			HsCreateNewMessage(phoneNumberP, nameP, NULL);
			break;
		}
		case EXIT_FOR_EMAIL:	//发送邮件
		{
			//构造参数
			phoneNumberP = (Char *)MemPtrNew(StrLen(actionParamP->phoneNumber) + 1);
			StrCopy(phoneNumberP, actionParamP->phoneNumber);
			MemPtrSetOwner(phoneNumberP, 0);
			
			//释放内存
			MemPtrFree(actionParamP);
			
			//调用邮件程序
			HsCreateNewEmail(phoneNumberP, NULL, NULL, NULL);
			break;
		}
		case EXIT_FOR_ADD_CONTACT:	//保存号码
		{
			AddrCreateNewRecordParamsType	*addContactParameterP	= (AddrCreateNewRecordParamsType *)MemPtrNew(sizeof(AddrCreateNewRecordParamsType));
			SysNotifyParamType				*notifyParamP			= (SysNotifyParamType *)MemPtrNew(sizeof(SysNotifyParamType));
		
			//构造调用参数
			addContactParameterP->data = (Char *)MemPtrNew(StrLen(actionParamP->phoneNumber) + 1);
			StrCopy(addContactParameterP->data, actionParamP->phoneNumber);
			MemPtrSetOwner(addContactParameterP->data, 0);
			MemPtrSetOwner(addContactParameterP, 0);
			notifyParamP->notifyType = addrAppNotificationCreateNewRecord;
			notifyParamP->broadcaster = APPLICATION_CREATOR;
			notifyParamP->notifyDetailsP = addContactParameterP;
			notifyParamP->handled = false;
			MemPtrSetOwner(notifyParamP, 0);
			
			//释放内存
			MemPtrFree(actionParamP);
			
			//打开通讯录
			if (settingP->treoModel == TREO_600 || settingP->treoModel == NOT_A_TREO)
			{
				addContactParameterP->field = 3;
				LaunchWithCommand('appl', 'addr', sysAppLaunchCmdNotify, notifyParamP);
			}
			else
			{
				addContactParameterP->field = 4;
				LaunchWithCommand('appl', 'PAdd', sysAppLaunchCmdNotify, notifyParamP);
			}
			break;
		}
		case EXIT_FOR_OPEN_CONTACT:	//打开联系人
		{
			UInt16				cardNo;
			LocalID				dbId;
			DmSearchStateType	dmSearchState;
			GoToParamsType		*gotoParameterP	= (GoToParamsType *)MemPtrNew(sizeof(GoToParamsType));
			
			//构造调用参数
			MemSet(gotoParameterP, sizeof(GoToParamsType), 0x00);
			MemPtrSetOwner(gotoParameterP, 0);
			gotoParameterP->recordNum = actionParamP->recordId;
			
			//释放内存
			MemPtrFree(actionParamP);
			
			//打开通讯录
			if (settingP->treoModel == TREO_600 || settingP->treoModel == NOT_A_TREO)
			{
				DmGetNextDatabaseByTypeCreator(true, &dmSearchState, 'DATA', 'addr', true, &cardNo, &dbId);
				gotoParameterP->dbID = dbId;
				LaunchWithCommand('appl', 'addr', sysAppLaunchCmdGoTo, gotoParameterP);
			}
			else
			{
				DmGetNextDatabaseByTypeCreator(true, &dmSearchState, 'DATA', 'PAdd', true, &cardNo, &dbId);
				gotoParameterP->dbID = dbId;
				LaunchWithCommand('appl', 'PAdd', sysAppLaunchCmdGoTo, gotoParameterP);
			}
			break;
		}
		default:	//不知名的情况
		{
			MemPtrFree(actionParamP);
			break;
		}
	}
}

#if 0
#pragma mark -
#endif

UInt32 PilotMain(UInt16 cmd, MemPtr cmdPBP, UInt16 launchFlags)
{
	SettingType		*settingP;
	
	switch (cmd)
	{
		case sysAppLaunchCmdNotify:
		{
			SysNotifyParamType	*notifyParamP = (SysNotifyParamType *)cmdPBP;
			switch (notifyParamP->notifyType)
			{
				case sysNotifyEventDequeuedEvent:
				{
					EventType	*eventP		= (EventType *)notifyParamP->notifyDetailsP;
					
					if (eventP->eType == NATIVE_KEYDOWN_EVENT)
					{
						Boolean		isNotifyHandled = false;
						
						settingP = (SettingType *)notifyParamP->userDataP;
						
						if (isEventShouldBePost(eventP, settingP, &isNotifyHandled))
						{
							UInt16				cardNo;
							LocalID				dbId;
							ActionParameterType	*actionParamP	= (ActionParameterType *)MemPtrNew(sizeof(ActionParameterType));
							
							notifyParamP->handled = true;
							MemSet(eventP, sizeof(EventType), 0x00);
							eventP->eType = nilEvent;
							
							SysCurAppDatabase(&cardNo, &dbId);
							SysNotifyUnregister(cardNo, dbId, sysNotifyEventDequeuedEvent, sysNotifyNormalPriority);
							
							MemSet(actionParamP, sizeof(ActionParameterType), 0x00);
							MemPtrSetOwner(actionParamP, 0);
							actionParamP->settingP = settingP;
							
							AppCallWithCommand(settingP->actionCreator, LAUNCH_CODE_ACTION, actionParamP);
							
							SysNotifyRegister(cardNo, dbId, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, settingP);
							
							if (actionParamP->hasActionToPerform)
							{
								performAction(actionParamP, settingP);
							}
							else
							{
								MemPtrFree(actionParamP);
							}
						}
						else if (isNotifyHandled)
						{
							notifyParamP->handled = true;
							MemSet(eventP, sizeof(EventType), 0x00);
							eventP->eType = nilEvent;
						}
					}
					
					break;
				}
				case sysNotifyDBDeletedEvent:
				{
					SysNotifyDBDeletedType	*dbInfo = (SysNotifyDBDeletedType *)notifyParamP->notifyDetailsP;
					
					settingP = (SettingType *)notifyParamP->userDataP;
					switch (dbInfo->creator)
					{
						case 'EzPH':
						{
							settingP->isEasyPhoneEnabled = false;
							PrefSetAppPreferences(APPLICATION_CREATOR, APPLICATION_PREF_ID, APPLICATION_PREF_VER, settingP, sizeof(SettingType), true);
							if (FtrGet('EzPH', 1, (UInt32 *)&settingP->skin.bmpBitsP) == errNone)
							{
								FtrPtrFree('EzPH', 1);
								settingP->skin.bmpWidth = 0;
								settingP->skin.bmpHeight = 0;
								settingP->skin.bmpDensity = kDensityLow;
								settingP->skin.bmpBitsP = NULL;
							}
							
							break;
						}
						case 'EzIP':
						{
							settingP->isEasyInputEnabled = false;
							PrefSetAppPreferences(APPLICATION_CREATOR, APPLICATION_PREF_ID, APPLICATION_PREF_VER, settingP, sizeof(SettingType), true);
							
							break;
						}
						case 'EzCT':
						{
							settingP->isEasyContactEnabled = false;
							PrefSetAppPreferences(APPLICATION_CREATOR, APPLICATION_PREF_ID, APPLICATION_PREF_VER, settingP, sizeof(SettingType), true);
							
							break;
						}
					}
					
					if (! (settingP->isEasyPhoneEnabled || settingP->isEasyInputEnabled || settingP->isEasyContactEnabled))
					{
						UInt16		cardNo;
						LocalID		dbId;
						
						SysCurAppDatabase(&cardNo, &dbId);
						SysNotifyUnregister(cardNo, dbId, sysNotifyEventDequeuedEvent, sysNotifyNormalPriority);
						SysNotifyUnregister(cardNo, dbId, sysNotifyDBDeletedEvent, sysNotifyNormalPriority);
					}
					
					break;
				}
				case sysNotifyVolumeMountedEvent:
				{
					UInt16			cardNo;
					LocalID			dbId;
					
					settingP = (SettingType *)notifyParamP->userDataP;
					
					SysCurAppDatabase(&cardNo, &dbId);
					if ((UInt8)settingP->skin.bmpPath[0] != 0)
					{
						if (loadSkinToFeature(settingP) == errNone)
						{
							SysNotifyRegister(cardNo, dbId, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, settingP);
							SysNotifyRegister(cardNo, dbId, sysNotifyDBDeletedEvent, NULL, sysNotifyNormalPriority, settingP);
						}
						else
						{
							FrmAlert(alertFileError);
						}
					}
					else
					{
						SysNotifyRegister(cardNo, dbId, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, settingP);
						SysNotifyRegister(cardNo, dbId, sysNotifyDBDeletedEvent, NULL, sysNotifyNormalPriority, settingP);
					}
					SysNotifyUnregister(cardNo, dbId, sysNotifyVolumeMountedEvent, sysNotifyNormalPriority);
					break;
				}
			}
			
			break;
		}
		case sysAppLaunchCmdPanelCalledFromApp:
		{
			frmMainEventHandler(true);
			
			break;
		}
		case sysAppLaunchCmdNormalLaunch:
		{
			frmMainEventHandler(false);
			
			break;
		}
		case sysAppLaunchCmdSystemReset:
		{
			UInt16			settingSize = sizeof(SettingType);
			UInt16			cardNo;
			LocalID			dbId;
			
			settingP = (SettingType *)MemPtrNew(sizeof(SettingType));
			if (PrefGetAppPreferences(APPLICATION_CREATOR, APPLICATION_PREF_ID, settingP, &settingSize, true) != noPreferenceFound)
			{
				if (settingP->isEasyPhoneEnabled || settingP->isEasyInputEnabled || settingP->isEasyContactEnabled)
				{
					FtrSet(APPLICATION_CREATOR, 0, ((UInt32)settingP));
					MemPtrSetOwner(settingP, 0);
					
					SysCurAppDatabase(&cardNo, &dbId);
					if (settingP->isEasyPhoneEnabled)
					{
						SysNotifyRegister(cardNo, dbId, sysNotifyVolumeMountedEvent, NULL, sysNotifyNormalPriority, settingP);
					}
					else
					{
						SysNotifyRegister(cardNo, dbId, sysNotifyEventDequeuedEvent, NULL, sysNotifyNormalPriority, settingP);
						SysNotifyRegister(cardNo, dbId, sysNotifyDBDeletedEvent, NULL, sysNotifyNormalPriority, settingP);
					}
				}
				else
				{
					MemPtrFree(settingP);
				}
			}
			else
			{
				MemPtrFree(settingP);
			}
			
			break;
		}
	}

	return errNone;
}
