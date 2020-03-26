using System;
using System.Text;
using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

namespace ConsoleApp
{
    class Program
    {
        private static readonly string MQTT_BROKER_ADDRESS = "broker.emqx.io";

        static void Main(string[] args)
        {
            Subscriber();

            Publisher();

            Subscriber();

            Console.ReadKey();
        }

        static void Subscriber()
        {
            // create client instance 
            MqttClient client = new MqttClient(MQTT_BROKER_ADDRESS);

            // register to message received 
            client.MqttMsgPublishReceived += client_MqttMsgPublishReceived;

            string clientId = Guid.NewGuid().ToString();
            client.Connect(clientId);
            
            client.Subscribe(new string[] { "home.temp" }, new byte[] { MqttMsgBase.QOS_LEVEL_AT_LEAST_ONCE });

        }

        static void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
        {
            Console.WriteLine(Encoding.UTF8.GetString(e.Message));
        }

        static void Publisher()
        {
            // create client instance 
            MqttClient client = new MqttClient(MQTT_BROKER_ADDRESS);

            string clientId = Guid.NewGuid().ToString();
            client.Connect(clientId);

            string strValue = Convert.ToString(25.5);
            
            var x = client.Publish("home.temp", Encoding.UTF8.GetBytes(strValue), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
        }
    }
}
