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
                if (!e.Message.Contains("Value could not be retrieved"))
                {
                    Console.WriteLine($"Error reading .env file: {e.Message}");
                    return;
                }

                Console.WriteLine("TOKEN not found in .env file.");
                Console.WriteLine("Recreate .env file? (overwrites existing file)");
                Console.Write("(y/n): ");

                var input = Console.ReadLine()?.Trim().ToLower();

                switch (input)
                {
                    case "y":
                        CreateEnvFile(envPath);
                        Console.WriteLine("File created. Add your token and restart the application.");
                        return;
                    case "n":
                        Console.WriteLine("File not created.");
                        Console.WriteLine("Manually add TOKEN=your_token_here to .env and restart.");
                        return;
                    default:
                        Console.WriteLine("Invalid input.");
                        Console.WriteLine("File not created.");
                        Console.WriteLine(
                            "Manually add TOKEN=your_token_here to .env and restart OR run the program and select to re-create the file.");
                        return;
                }
            }


            if (string.IsNullOrEmpty(token) || token == "PASTE_YOUR_TOKEN")
            {
                Console.WriteLine($"Add your token in {envPath}");
                return;
            }
        }
    }
}