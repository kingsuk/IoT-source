using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using ESPWebCore.Models;
using uPLibrary.Networking.M2Mqtt;
using System.Net;
using uPLibrary.Networking.M2Mqtt.Messages;
using System.Text;

namespace ESPWebCore.Controllers
{
    public class HomeController : Controller
    {
        public IActionResult Index()
        {
            return View();
        }

        public IActionResult About()
        {
            ViewData["Message"] = "Your application description page.";

            return View();
        }

        public IActionResult Contact()
        {
            ViewData["Message"] = "Your contact page.";

            return View();
        }

        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }

        [HttpPost]
        public ActionResult publishTopic(string message)
        {

            MqttClient client = new MqttClient("139.59.28.88");

            client.MqttMsgSubscribed += client_MqttMsgSubscribed;
            client.MqttMsgUnsubscribed += client_MqttMsgUnsubscribed;

            client.MqttMsgPublishReceived += client_MqttMsgPublishReceived;

            client.MqttMsgPublished += client_MqttMsgPublished;

            client.Connect(Guid.NewGuid().ToString());

            string topic =  "inTopic/1002" ;

            byte[] qosLevels = { MqttMsgBase.QOS_LEVEL_AT_MOST_ONCE };
            //client.Subscribe(topic, qosLevels);

            string jsonDataToSend = message;

            client.Publish(topic, Encoding.UTF8.GetBytes(jsonDataToSend));

            //client.Unsubscribe(topic);

            return Content("call done");
        }

        private static void client_MqttMsgPublished(object sender, MqttMsgPublishedEventArgs e)
        {
            Console.WriteLine("Message is published " + e.MessageId.ToString());
            throw new NotImplementedException();
        }

        private static void client_MqttMsgSubscribed(object sender, MqttMsgSubscribedEventArgs e)
        {
            Console.WriteLine("Message Subscribed " + e.MessageId.ToString());
            //throw new NotImplementedException();
        }

        private static void client_MqttMsgUnsubscribed(object sender, MqttMsgUnsubscribedEventArgs e)
        {
            Console.WriteLine("Message UnSubscribed " + e.MessageId.ToString());
            //throw new NotImplementedException();
        }

        private static void client_MqttMsgPublishReceived(object sender, MqttMsgPublishEventArgs e)
        {
            Console.WriteLine("Message received " + System.Text.Encoding.UTF8.GetString(e.Message));
            //throw new NotImplementedException();
        }
    }
}
