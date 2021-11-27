# Two-Wheels-Inverted-Pendulum-with-Linear-Quadratic-Integral-Control-
Code to a TWIP (Self Balancing Robot) based on WEMOS R1 D32 (ESP32) board, using tb6612fng Bridge, jgb37-520b (178 RPM, 12V) DC motor and Hall Magnetic Encoder, MPU6050. It uses 2 controllers, a LQR to rotation control and a LQI to stability and linear position control.

The code was done in platformIO + Vscode, but you can just rename "src" folder to "main" and rename "main.cpp" to "main.ino", and run it on arduino IDE. 

I use this robot, from aliexpress:
https://pt.aliexpress.com/item/4001193841954.html?spm=a2g0o.productlist.0.0.62904269Z7r5GP&algo_pvid=7072c17d-7309-47f6-8868-ad8df2cf37c3&algo_exp_id=7072c17d-7309-47f6-8868-ad8df2cf37c3-5&pdp_ext_f=%7B%22sku_id%22%3A%2210000015262081999%22%7D

But, of course, i replace the Uno Board to a Wemos D1 R32 (Esp32). 

I did a controller (joystick) using HTML, so you just need start the robot, see the IP and put it on a Web Browser. I did too the same in python. 

![Alt text](https://github.com/suelioalencar/Two-Wheels-Inverted-Pendulum-with-Linear-Quadratic-Integral-Control-/blob/main/Python%20Joystick/Joystick%20Pyton.png?raw=true "Python Joystick")
