import streamlit as st
import requests
import pandas as pd
import matplotlib.pyplot as plt
import time

SERVER_URL = "http://192.168.80.200:5000/get_data" 

st.title("ESP32 Sensor Data Dashboard")
st.subheader("Real-time Data Visualization")

ecg_plot = st.empty()
mpu_plot = st.empty()

while True:
    try:
        response = requests.get(SERVER_URL)
        if response.status_code == 200:
            data = response.json()
            df = pd.DataFrame(data)

            if not df.empty:
                df['timestamp'] = pd.to_datetime(df['timestamp'], unit='s')

                ecg_plot.line_chart(df.tail(50)[["ecg"]])

                fig, ax = plt.subplots(2, 1, figsize=(10, 6))
                ax[0].plot(df["timestamp"], df["acc_x"], label="AccX")
                ax[0].plot(df["timestamp"], df["acc_y"], label="AccY")
                ax[0].plot(df["timestamp"], df["acc_z"], label="AccZ")
                ax[0].set_title("Acceleration")
                ax[0].legend()

                ax[1].plot(df["timestamp"], df["gyro_x"], label="GyroX")
                ax[1].plot(df["timestamp"], df["gyro_y"], label="GyroY")
                ax[1].plot(df["timestamp"], df["gyro_z"], label="GyroZ")
                ax[1].set_title("Gyroscope")
                ax[1].legend()

                mpu_plot.pyplot(fig)

            time.sleep(0.5)
    except Exception as e:
        st.error(f"Error: {e}")
