package com.example.bluetoothandroid.IoTPractice.Bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;

import com.example.bluetoothandroid.IoTPractice.data.Constant;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class BTService {

    // Bluetooth 상태
    public static final int STATE_NONE = 0;
    public static final int STATE_CONNECTING = 1;
    public static final int STATE_CONNECTED = 2;

    // Bluetooth Serial Port Profile UUID
    private static final UUID MY_UUID =
            UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    private final BluetoothAdapter mAdapter;
    private final Handler mHandler;

    private ConnectThread mConnectThread;
    private ConnectedThread mConnectedThread;

    private int mState = STATE_NONE;

    public BTService(BluetoothAdapter adapter, Handler handler) {
        mAdapter = adapter;
        mHandler = handler;
    }

    // 현재 Bluetooth 상태
    public synchronized int getState() {
        return mState;
    }

    // 상태 변경
    private synchronized void setState(int state) {
        mState = state;

        Message msg = mHandler.obtainMessage(
                Constant.MESSAGE_STATE_CHANGE,
                state,
                -1
        );

        msg.sendToTarget();
    }

    // 연결 시작
    public synchronized void start() {

        // 기존 연결 작업 종료
        if (mConnectThread != null) {
            mConnectThread.cancel();
            mConnectThread = null;
        }

        // 기존 통신 작업 종료
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }

        setState(STATE_NONE);
    }

    // Bluetooth 장치 연결
    public synchronized void connect(BluetoothDevice device) {

        // 기존 연결 시도 종료
        if (mState == STATE_CONNECTING) {
            if (mConnectThread != null) {
                mConnectThread.cancel();
                mConnectThread = null;
            }
        }

        // 기존 연결 종료
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }

        // 새로운 연결 시도
        mConnectThread = new ConnectThread(device);
        mConnectThread.start();

        setState(STATE_CONNECTING);
    }

    // 연결 성공 후 처리
    private synchronized void connected(
            BluetoothSocket socket,
            BluetoothDevice device
    ) {

        // 연결 시도 종료
        if (mConnectThread != null) {
            mConnectThread.cancel();
            mConnectThread = null;
        }

        // 기존 통신 종료
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }

        // 새로운 통신 시작
        mConnectedThread = new ConnectedThread(socket);
        mConnectedThread.start();

        setState(STATE_CONNECTED);

        // 연결된 장치 이름 전달
        Message msg = mHandler.obtainMessage(
                Constant.MESSAGE_DEVICE_NAME
        );

        Bundle bundle = new Bundle();
        bundle.putString(
                Constant.DEVICE_NAME,
                device.getName()
        );

        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    // 연결 실패
    private void connectionFailed() {

        setState(STATE_NONE);

        Message msg = mHandler.obtainMessage(
                Constant.MESSAGE_TOAST
        );

        Bundle bundle = new Bundle();
        bundle.putString(
                Constant.TOAST,
                "Bluetooth 연결에 실패했습니다."
        );

        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    // 연결 종료
    private void connectionLost() {

        setState(STATE_NONE);

        Message msg = mHandler.obtainMessage(
                Constant.MESSAGE_TOAST
        );

        Bundle bundle = new Bundle();
        bundle.putString(
                Constant.TOAST,
                "Bluetooth 연결이 끊어졌습니다."
        );

        msg.setData(bundle);
        mHandler.sendMessage(msg);
    }

    // Bluetooth 서비스 종료
    public synchronized void stop() {

        if (mConnectThread != null) {
            mConnectThread.cancel();
            mConnectThread = null;
        }

        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }

        setState(STATE_NONE);
    }

    // 데이터 전송
    public void write(byte[] out) {

        ConnectedThread r;

        synchronized (this) {

            if (mState != STATE_CONNECTED) {
                return;
            }

            r = mConnectedThread;
        }

        r.write(out);
    }


    // =========================================================
    // 연결 시도 Thread
    // =========================================================

    private class ConnectThread extends Thread {

        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;

        public ConnectThread(BluetoothDevice device) {

            mmDevice = device;

            BluetoothSocket tmp = null;

            try {

                tmp = device.createRfcommSocketToServiceRecord(
                        MY_UUID
                );

            } catch (IOException e) {
                e.printStackTrace();
            }

            mmSocket = tmp;
        }

        @Override
        public void run() {

            // 검색 중이면 취소
            mAdapter.cancelDiscovery();

            try {

                if (mmSocket != null) {

                    mmSocket.connect();

                    synchronized (BTService.this) {
                        mConnectThread = null;
                    }

                    connected(mmSocket, mmDevice);
                }

            } catch (IOException e) {

                try {

                    if (mmSocket != null) {
                        mmSocket.close();
                    }

                } catch (IOException closeException) {
                    closeException.printStackTrace();
                }

                connectionFailed();
            }
        }

        public void cancel() {

            try {

                if (mmSocket != null) {
                    mmSocket.close();
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }


    // =========================================================
    // 연결 후 데이터 송수신 Thread
    // =========================================================

    private class ConnectedThread extends Thread {

        private final BluetoothSocket mmSocket;

        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {

            mmSocket = socket;

            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {

                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();

            } catch (IOException e) {
                e.printStackTrace();
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }

        @Override
        public void run() {

            byte[] buffer = new byte[1024];

            int bytes;

            while (true) {

                try {

                    if (mmInStream == null) {
                        break;
                    }

                    bytes = mmInStream.read(buffer);

                    if (bytes > 0) {

                        Message msg = mHandler.obtainMessage(
                                Constant.MESSAGE_READ,
                                bytes,
                                -1,
                                buffer.clone()
                        );

                        msg.sendToTarget();
                    }

                } catch (IOException e) {

                    connectionLost();
                    break;
                }
            }
        }

        // 데이터 전송
        public void write(byte[] buffer) {

            try {

                if (mmOutStream != null) {

                    mmOutStream.write(buffer);

                    Message msg = mHandler.obtainMessage(
                            Constant.MESSAGE_WRITE,
                            -1,
                            -1,
                            buffer
                    );

                    msg.sendToTarget();
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        // 연결 종료
        public void cancel() {

            try {

                if (mmSocket != null) {
                    mmSocket.close();
                }

            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}