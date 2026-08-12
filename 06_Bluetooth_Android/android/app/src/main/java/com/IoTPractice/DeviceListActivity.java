package com.example.bluetoothandroid.IoTPractice;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Set;

public class DeviceListActivity extends Activity {

    // p5Activity에서 사용하는 주소 전달용 key
    public static final String EXTRA_DEVICE_ADDRESS = "device_address";

    private BluetoothAdapter mBluetoothAdapter;
    private ArrayAdapter<String> mPairedDevicesArrayAdapter;

    private ArrayList<BluetoothDevice> mDevices = new ArrayList<>();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // 리스트 화면
        ListView listView = new ListView(this);
        setContentView(listView);

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth를 지원하지 않는 기기입니다.",
                    Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        // 페어링된 Bluetooth 장치 목록
        mPairedDevicesArrayAdapter =
                new ArrayAdapter<>(this,
                        android.R.layout.simple_list_item_1);

        listView.setAdapter(mPairedDevicesArrayAdapter);

        showPairedDevices();

        // 장치 선택
        listView.setOnItemClickListener((parent, view, position, id) -> {

            BluetoothDevice device = mDevices.get(position);

            String address = device.getAddress();

            // 선택한 장치의 MAC 주소를 p5Activity로 전달
            Intent intent = new Intent();

            intent.putExtra(EXTRA_DEVICE_ADDRESS, address);

            setResult(Activity.RESULT_OK, intent);

            finish();
        });
    }

    private void showPairedDevices() {

        Set<BluetoothDevice> pairedDevices =
                mBluetoothAdapter.getBondedDevices();

        if (pairedDevices.size() > 0) {

            for (BluetoothDevice device : pairedDevices) {

                mDevices.add(device);

                String deviceName = device.getName();
                String deviceAddress = device.getAddress();

                mPairedDevicesArrayAdapter.add(
                        deviceName + "\n" + deviceAddress
                );
            }

        } else {

            mPairedDevicesArrayAdapter.add(
                    "페어링된 Bluetooth 장치가 없습니다."
            );
        }
    }
}