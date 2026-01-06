using dotenv.net;

namespace STRSTeamsCheckIn
{
    internal static class Program
    {
        private static void CreateEnvFile(string envFilePath)
        {
            File.WriteAllText(envFilePath, "TOKEN=PASTE_YOUR_TOKEN");
            Console.WriteLine($"Created env file. Please add your token in {envFilePath}");
        }
        private static void Main()
        {
            var envPath = Path.Combine(AppContext.BaseDirectory, ".env");
            
            if (!File.Exists(envPath))
            {
                CreateEnvFile(envPath);
                return;
            }
            
            DotEnv.Load(options: new DotEnvOptions(envFilePaths: [envPath]));
            
            var token = Environment.GetEnvironmentVariable("TOKEN");
            
            if (string.IsNullOrEmpty(token) || token == "PASTE_YOUR_TOKEN")
            {
                Console.WriteLine($"Please add your token in {envPath}");
                return;
            }
            
        }
    }
}