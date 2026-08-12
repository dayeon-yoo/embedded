package com.example.bluetoothandroid.IoTPractice.data;

public class Constant {

    // Bluetooth 상태 변경
    public static final int MESSAGE_STATE_CHANGE = 1;

    // Bluetooth 데이터 수신
    public static final int MESSAGE_READ = 2;

    // Bluetooth 데이터 전송
    public static final int MESSAGE_WRITE = 3;

    // 연결된 Bluetooth 장치 이름
    public static final int MESSAGE_DEVICE_NAME = 4;

    // Toast 메시지
    public static final int MESSAGE_TOAST = 5;

    // 이미지 로딩
    public static final int MESSAGE_START_IMAGE = 6;


    // Intent / Bundle에서 사용하는 Key
    public static final String DEVICE_NAME = "device_name";

    public static final String TOAST = "toast";
}