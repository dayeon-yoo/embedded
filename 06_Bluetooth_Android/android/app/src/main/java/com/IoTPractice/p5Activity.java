package com.example.bluetoothandroid.IoTPractice;

import android.Manifest;
import android.app.Activity;
import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.core.app.ActivityCompat;

import com.example.bluetoothandroid.R;
import com.example.bluetoothandroid.IoTPractice.Bluetooth.BTService;
import com.example.bluetoothandroid.IoTPractice.data.Constant;


import java.lang.ref.WeakReference;


public class p5Activity extends Activity {

    private static final String TAG = "p5Activity";
    private static final boolean D = true;

    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;

    private BluetoothAdapter mBluetoothAdapter = null;
    private BTService mBTService = null;

    private String mConnectedDeviceName = null;

    // Bluetooth 버튼
    private Button mBtBTN;

    // DC / Servo 버튼
    private Button mUpBtn;
    private Button mDownBtn;
    private Button mLeftBtn;
    private Button mRightBtn;

    // 속도 버튼
    private Button mAccBtn;
    private Button mDecBtn;
    private Button mStopBtn;
    private Button mSpeedBtn;

    // 속도 표시
    private TextView mSpeedText;

    private ProgressDialog mProgress;
    private Resources mAppRes;

    private final Handler mHandler = new MessageHandler(this);


    // =========================================================
    // onCreate
    // =========================================================
    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_p5);

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (mBluetoothAdapter == null) {

            Toast.makeText(
                    this,
                    "Bluetooth is not available",
                    Toast.LENGTH_LONG
            ).show();

            finish();
        }
    }


    // =========================================================
    // onStart
    // =========================================================
    @Override
    public void onStart() {

        super.onStart();

        if (D)
            Log.e(TAG, "++ ON START ++");


        if (!mBluetoothAdapter.isEnabled()) {

            if (ActivityCompat.checkSelfPermission(
                    this,
                    Manifest.permission.BLUETOOTH_CONNECT
            ) != PackageManager.PERMISSION_GRANTED) {

                return;
            }

            startActivityForResult(
                    new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE),
                    REQUEST_ENABLE_BT
            );

        } else {

            if (mBTService == null)
                initUI();
        }
    }


    // =========================================================
    // onResume
    // =========================================================
    @Override
    public synchronized void onResume() {

        super.onResume();

        if (D)
            Log.e(TAG, "+ ON RESUME +");


        if (mBTService != null &&
                mBTService.getState() == BTService.STATE_NONE) {

            mBTService.start();
        }
    }


    // =========================================================
    // onDestroy
    // =========================================================
    @Override
    public void onDestroy() {

        super.onDestroy();

        if (mBTService != null)
            mBTService.stop();

        dismissProgress();
    }


    // =========================================================
    // XML 버튼 클릭 처리
    // =========================================================
    public void clickFunc(View v) {

        switch (v.getId()) {


            // -------------------------------------------------
            // Bluetooth 연결
            // -------------------------------------------------
            case R.id.btBTN:

                if (mBTService != null)
                    mBTService.stop();

                startActivityForResult(
                        new Intent(this, DeviceListActivity.class),
                        REQUEST_CONNECT_DEVICE
                );

                break;


            // -------------------------------------------------
            // 속도 확인
            // -------------------------------------------------
            case R.id.p5_speedCheck:

                sendMessage("@SPD,#");

                break;


            // -------------------------------------------------
            // DC 모터 정방향
            // -------------------------------------------------
            case R.id.p5_up:

                sendMessage("@MCW,#");

                break;


            // -------------------------------------------------
            // DC 모터 역방향
            // -------------------------------------------------
            case R.id.p5_down:

                sendMessage("@MCC,#");

                break;


            // -------------------------------------------------
            // 서보 왼쪽
            // -------------------------------------------------
            case R.id.p5_left:

                sendMessage("@SVL,#");

                break;


            // -------------------------------------------------
            // 서보 오른쪽
            // -------------------------------------------------
            case R.id.p5_right:

                sendMessage("@SVR,#");

                break;


            // -------------------------------------------------
            // 가속
            // -------------------------------------------------
            case R.id.p5_accel:

                sendMessage("@ACC,#");

                break;


            // -------------------------------------------------
            // 감속
            // -------------------------------------------------
            case R.id.p5_decel:

                sendMessage("@DEC,#");

                break;


            // -------------------------------------------------
            // 정지
            // -------------------------------------------------
            case R.id.p5_stop:

                sendMessage("@STP,#");

                break;
        }
    }


    // =========================================================
    // UI 초기화
    // =========================================================
    private void initUI() {

        mAppRes = this.getResources();


        // Bluetooth
        mBtBTN = findViewById(R.id.btBTN);


        // DC Motor
        mUpBtn = findViewById(R.id.p5_up);
        mDownBtn = findViewById(R.id.p5_down);


        // Servo
        mLeftBtn = findViewById(R.id.p5_left);
        mRightBtn = findViewById(R.id.p5_right);


        // Speed
        mAccBtn = findViewById(R.id.p5_accel);
        mDecBtn = findViewById(R.id.p5_decel);
        mStopBtn = findViewById(R.id.p5_stop);
        mSpeedBtn = findViewById(R.id.p5_speedCheck);


        // Speed TextView
        mSpeedText = findViewById(R.id.p5_speedText);


        // Bluetooth Service
        mBTService = new BTService(this, mHandler);
    }


    // =========================================================
    // Bluetooth 메시지 전송
    // =========================================================
    private void sendMessage(String message) {

        if (mBTService == null)
            return;


        if (mBTService.getState() !=
                BTService.STATE_CONNECTED) {

            Toast.makeText(
                    this,
                    R.string.not_connected,
                    Toast.LENGTH_SHORT
            ).show();

            return;
        }


        if (!message.isEmpty()) {

            mBTService.write(
                    message.getBytes()
            );
        }
    }


    // =========================================================
    // Bluetooth 장치 연결
    // =========================================================
    private void connectDevice(Intent data) {

        String address =
                data.getExtras().getString(
                        DeviceListActivity.EXTRA_DEVICE_ADDRESS
                );


        BluetoothDevice device =
                mBluetoothAdapter.getRemoteDevice(address);


        mBTService.connect(device);
    }


    // =========================================================
    // Raspberry Pi → Android
    //
    // 수신 예:
    // @SPD,50#
    // =========================================================
    private void setResultValue(String readMessage) {

        if (readMessage == null)
            return;


        int start =
                readMessage.indexOf("@SPD,");

        int end =
                readMessage.indexOf("#", start);


        if (start != -1 && end != -1) {

            String value =
                    readMessage.substring(
                            start + 5,
                            end
                    );


            mSpeedText.setText(
                    "Speed : " + value
            );
        }
    }


    // =========================================================
    // ProgressDialog
    // =========================================================
    private void showProgressDialog(String type) {

        if (mProgress == null) {

            mProgress =
                    new ProgressDialog(this);

            mProgress.setCancelable(false);

            mProgress.setMessage(
                    "[ " + type + " ] Connecting..."
            );

            mProgress.show();
        }
    }


    private void dismissProgress() {

        if (mProgress != null &&
                mProgress.isShowing()) {

            mProgress.dismiss();

            mProgress = null;
        }
    }


    // =========================================================
    // Bluetooth Handler
    // =========================================================
    private static class MessageHandler
            extends Handler {

        private final WeakReference<p5Activity>
                activityRef;


        public MessageHandler(p5Activity activity) {

            activityRef =
                    new WeakReference<>(activity);
        }


        @Override
        public void handleMessage(Message msg) {

            p5Activity activity =
                    activityRef.get();


            if (activity == null)
                return;


            switch (msg.what) {


                // ---------------------------------------------
                // Bluetooth 상태 변경
                // ---------------------------------------------
                case Constant.MESSAGE_STATE_CHANGE:

                    switch (msg.arg1) {


                        case BTService.STATE_CONNECTED:

                            activity.mBtBTN.setSelected(true);

                            activity.dismissProgress();

                            break;


                        case BTService.STATE_CONNECTING:

                            activity.showProgressDialog(
                                    "Bluetooth"
                            );

                            break;


                        case BTService.STATE_NONE:

                            activity.mBtBTN.setSelected(false);

                            break;
                    }

                    break;


                // ---------------------------------------------
                // Raspberry Pi → Android 데이터 수신
                // ---------------------------------------------
                case Constant.MESSAGE_READ:

                    byte[] readBuf =
                            (byte[]) msg.obj;


                    String readMessage =
                            new String(
                                    readBuf,
                                    0,
                                    msg.arg1
                            );


                    activity.setResultValue(
                            readMessage
                    );

                    break;


                // ---------------------------------------------
                // 연결된 장치 이름
                // ---------------------------------------------
                case Constant.MESSAGE_DEVICE_NAME:

                    activity.mConnectedDeviceName =
                            msg.getData().getString(
                                    Constant.DEVICE_NAME
                            );


                    Toast.makeText(
                            activity,
                            "Connected to " +
                                    activity.mConnectedDeviceName,
                            Toast.LENGTH_SHORT
                    ).show();

                    break;


                // ---------------------------------------------
                // Toast
                // ---------------------------------------------
                case Constant.MESSAGE_TOAST:

                    Toast.makeText(
                            activity,
                            activity.mAppRes.getString(
                                    msg.getData().getInt(
                                            Constant.TOAST
                                    )
                            ),
                            Toast.LENGTH_SHORT
                    ).show();

                    break;
            }
        }
    }
}