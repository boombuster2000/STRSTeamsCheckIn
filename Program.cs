
namespace STRSTeamsCheckIn
{
    internal static class Program
    {
        private static void CreateEnvFile()
        {
            File.WriteAllText("./.env", "TOKEN=YOUR_TOKEN");
            Console.WriteLine("Created env file. Please add your token.");
        }
        private static void Main()
        {
            if (!File.Exists("./.env"))
                CreateEnvFile();
            
        }
    }
}