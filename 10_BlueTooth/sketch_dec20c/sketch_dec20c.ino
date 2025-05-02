
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // 服务UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E" // 接收特征码
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E" // 发送特征码

//服务监听
class MyServerCallbacks: public BLEServerCallbacks {
    //当连接成功
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };
    //当连接断开
    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

//信息监听
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      //保存接收到的数据
      std::string rxValue = pCharacteristic->getValue();
      //如果接收的数据大于 0
      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
        //输出接收到的数据
        for (int i = 0; i < rxValue.length(); i++)
          Serial.print(rxValue[i]);
        //换行
        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  //开启串口0 波特率为115200
  Serial.begin(115200);

  // 创建蓝牙设备，名称为LCKFB
  BLEDevice::init("LCKFB");

  // 创建蓝牙服务器
  pServer = BLEDevice::createServer();
  // 设置监听
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  //设置蓝牙服务UUID
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // 设置发送特征码
  pTxCharacteristic = pService->createCharacteristic(
                    CHARACTERISTIC_UUID_TX,
                    BLECharacteristic::PROPERTY_NOTIFY
                  );

  // 设置描述符
  pTxCharacteristic->addDescriptor(new BLE2902());

  // 设置接收特征码
  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
                       CHARACTERISTIC_UUID_RX,
                      BLECharacteristic::PROPERTY_WRITE
                    );

  //设置信息监听
  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // 启动服务器
  pService->start();

  // 开启广播
  pServer->getAdvertising()->start();

  //串口提示
  Serial.println("Waiting a client connection to notify...");
}

void loop() {

    //当有设备连接成功
    if (deviceConnected) {
        //设置传输特征值为 txValue （就是发送的数据来源是txValue）
        pTxCharacteristic->setValue(&txValue, 1);
        //发送通知（发送数据）
        pTxCharacteristic->notify();
        //值自增(范围0~255)
        txValue++;
        // 如果发送的数据包太多，蓝牙堆栈将进入拥塞状态，所以加个延时
        delay(100);
  }
    // 当有设备断开连接后
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // 让蓝牙堆栈有机会做好准备
        pServer->startAdvertising(); //重新启动广播
        //串口提示开启广播
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // 当有设备连接成功后
    if ( deviceConnected && !oldDeviceConnected ) {
      //串口提示连接成功
      Serial.println("connecting....");
    // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
}