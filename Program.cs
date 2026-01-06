using dotenv.net;

namespace STRSTeamsCheckIn
{
    internal static class Program
    {
        private static void CreateEnvFile()
        {
            File.WriteAllText("./.env", "TOKEN=PASTE_YOUR_TOKEN");
            Console.WriteLine("Created env file. Please add your token in ./.env.");
        }
        private static void Main()
        {
            if (!File.Exists("./.env"))
            {
                CreateEnvFile();
                return;
            }
            
            DotEnv.Load();
            
            var token = Environment.GetEnvironmentVariable("TOKEN");
            
            if (string.IsNullOrEmpty(token) || token == "PASTE_YOUR_TOKEN")
            {
                Console.WriteLine("Please add your token in ./.env.");
                return;
            }
            
        }
    }
}