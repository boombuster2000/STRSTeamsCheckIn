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

                Console.WriteLine($"Created env file. Add your token in {envFilePath} and restart the program.");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Failed to create .env file: {ex.Message}");
                throw;
            }
        }

        private static bool TryGetToken(string envFilePath, out string outToken)
        {
            outToken = "";

            if (!File.Exists(envFilePath))
            {
                CreateEnvFile(envFilePath);
                return false;
            }

            DotEnv.Load(options: new DotEnvOptions(envFilePaths: [envFilePath]));

            if (!EnvReader.TryGetStringValue("TOKEN", out var token))
            {
                Console.WriteLine("TOKEN not found in .env file.");
                Console.WriteLine("Recreate .env file? (overwrites existing file)");
                Console.Write("(y/n): ");

                var input = Console.ReadLine()?.Trim().ToLower();

                switch (input)
                {
                    case "y":
                        CreateEnvFile(envFilePath);
                        return false;
                    case "n":
                        Console.WriteLine("File not created.");
                        Console.WriteLine(
                            "Manually add TOKEN=your_token_here to .env and restart OR run the program and select re-create the file.");
                        return false;
                    default:
                        Console.WriteLine("Invalid input.");
                        Console.WriteLine("File not created.");
                        Console.WriteLine(
                            "Manually add TOKEN=your_token_here to .env and restart OR run the program and select re-create the file.");
                        return false;
                }
            }

            if (string.IsNullOrEmpty(token) || token == "PASTE_YOUR_TOKEN")
            {
                Console.WriteLine($"Add your token in {envFilePath} and restart the program.");
                return false;
            }

            outToken = token;
            return true;
        }

        private static void Main()
        {
            string token;
            if (!TryGetToken(Path.Combine(AppContext.BaseDirectory, ".env"), out token))
                return;
        }
    }
}