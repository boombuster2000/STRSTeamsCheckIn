
namespace STRSTeamsCheckIn
{
    internal static class Program
    {
        private static void CreateEnvFile()
        {
            File.WriteAllText("./.env", "TOKEN=PASTE_YOUR_TOKEN");
            Console.WriteLine("Created env file. Please add your token.");
        }
        private static void Main()
        {
            if (!File.Exists("./.env"))
            {
                CreateEnvFile();
                return;
            }
            else if (File.ReadAllText("./.env") == "TOKEN=PASTE_YOUR_TOKEN")
            {
                Console.WriteLine("Token is still its default value. Please add your token.");
                return;
            }
            
        }
    }
}