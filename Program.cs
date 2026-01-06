using dotenv.net;
using dotenv.net.Utilities;

namespace STRSTeamsCheckIn
{
    internal static class Program
    {
        private static void CreateEnvFile(string envFilePath)
        {
            try
            {
                File.WriteAllText(envFilePath, "TOKEN=PASTE_YOUR_TOKEN");

                // Set restrictive permissions on Unix-like systems
                if (!OperatingSystem.IsWindows())
                    File.SetUnixFileMode(envFilePath, UnixFileMode.UserRead | UnixFileMode.UserWrite);

                Console.WriteLine($"Created env file. Add your token in {envFilePath}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to create .env file: {ex.Message}");
                throw;
            }
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

            string token;

            try
            {
                token = EnvReader.GetStringValue("TOKEN");
            }
            catch (Exception e)
            {
                if (e.Message.Contains("Value could not be retrieved"))
                {
                    Console.WriteLine("TOKEN could not be retrieved");
                    CreateEnvFile(envPath);
                }
                else
                {
                    Console.WriteLine(e.Message);
                }

                return;
            }


            if (string.IsNullOrEmpty(token) || token == "PASTE_YOUR_TOKEN")
            {
                Console.WriteLine($"Add your token in {envPath}");
                return;
            }
        }
    }
}