#include <RobotMap.h>

std::shared_ptr<can::TalonSRX> RobotMap::intakeSubsystemLeftMotor;
std::shared_ptr<can::TalonSRX> RobotMap::intakeSubsystemRightMotor;

std::shared_ptr<can::TalonSRX> RobotMap::elevatorClimberSubsystemPrimaryTalon;
std::shared_ptr<can::TalonSRX> RobotMap::elevatorClimberSubsystemFollower01Talon;
std::shared_ptr<can::TalonSRX> RobotMap::elevatorClimberSubsystemFollower02Talon;

std::shared_ptr<can::TalonSRX> RobotMap::swerveSubsystemFLDriveTalon;
std::shared_ptr<can::TalonSRX> RobotMap::swerveSubsystemFRDriveTalon;
std::shared_ptr<can::TalonSRX> RobotMap::swerveSubsystemBLDriveTalon;
std::shared_ptr<can::TalonSRX> RobotMap::swerveSubsystemBRDriveTalon;

std::shared_ptr<can::TalonSRX> RobotMap::swerveSubsystemFLRotTalon;
std::shared_ptr<can::TalonSRX> RobotMap::swerveSubsystemFRRotTalon;
std::shared_ptr<can::TalonSRX> RobotMap::swerveSubsystemBLRotTalon;
std::shared_ptr<can::TalonSRX> RobotMap::swerveSubsystemBRRotTalon;

std::shared_ptr<frc::DoubleSolenoid> RobotMap::climberSubsystemLatchSolenoid;
std::shared_ptr<frc::DoubleSolenoid> RobotMap::climberSubsystemWingSolenoid;
std::shared_ptr<frc::DoubleSolenoid> RobotMap::elevatorClimberSubsystemShifterSolenoid;

std::shared_ptr<AHRS> RobotMap::robotIMU;
std::shared_ptr<frc::PowerDistributionPanel> RobotMap::powerDistributionPanel;
std::shared_ptr<TigerDrive> RobotMap::tigerDrive;
std::shared_ptr<TigerSwerve> RobotMap::tigerSwerve;
std::vector<std::shared_ptr<can::TalonSRX>> RobotMap::talonVector;

int RobotMap::TOP_POSITION_TICKS;
double RobotMap::GROUND_POS_FT;
double RobotMap::SCALE_POS_FT;
double RobotMap::SWITCH_POS_FT;
double RobotMap::WHEELBASE_WIDTH;
double RobotMap::WHEELBASE_LENGTH;

void RobotMap::init() {
	std::cout << "RobotMap is starting!" << std::endl;

	TOP_POSITION_TICKS = 24000;
	//FEET
	GROUND_POS_FT = 0;
	SCALE_POS_FT = 6.5;
	SWITCH_POS_FT = 1.75;
	//INCHES - don't question it
	WHEELBASE_LENGTH = 23;
	WHEELBASE_WIDTH = 21.875;

	intakeSubsystemLeftMotor.reset(new TalonSRX(10));
	intakeSubsystemRightMotor.reset(new TalonSRX(11));

	elevatorClimberSubsystemPrimaryTalon.reset(new TalonSRX(13));
	elevatorClimberSubsystemFollower01Talon.reset(new TalonSRX(14));
	elevatorClimberSubsystemFollower02Talon.reset(new TalonSRX(15));

	swerveSubsystemFLDriveTalon.reset(new can::TalonSRX(2));
	swerveSubsystemFRDriveTalon.reset(new can::TalonSRX(3));
	swerveSubsystemBLDriveTalon.reset(new can::TalonSRX(4));
	swerveSubsystemBRDriveTalon.reset(new can::TalonSRX(5));

	swerveSubsystemFLRotTalon.reset(new can::TalonSRX(6));
	swerveSubsystemFRRotTalon.reset(new can::TalonSRX(7));
	swerveSubsystemBLRotTalon.reset(new can::TalonSRX(8));
	swerveSubsystemBRRotTalon.reset(new can::TalonSRX(9));

	climberSubsystemLatchSolenoid.reset(new frc::DoubleSolenoid(0, 1));
	climberSubsystemWingSolenoid.reset(new frc::DoubleSolenoid(2, 3));
	elevatorClimberSubsystemShifterSolenoid.reset(new frc::DoubleSolenoid(4, 5));

	talonVector.push_back(swerveSubsystemFLDriveTalon);
	talonVector.push_back(swerveSubsystemFRDriveTalon);
	talonVector.push_back(swerveSubsystemBLDriveTalon);
	talonVector.push_back(swerveSubsystemBRDriveTalon);

	talonVector.push_back(swerveSubsystemFLRotTalon);
	talonVector.push_back(swerveSubsystemFRRotTalon);
	talonVector.push_back(swerveSubsystemBLRotTalon);
	talonVector.push_back(swerveSubsystemBRRotTalon);

	robotIMU.reset(new AHRS(frc::SPI::Port::kMXP));
	powerDistributionPanel.reset(new frc::PowerDistributionPanel());
	tigerSwerve.reset(new TigerSwerve(RobotMap::talonVector));
	tigerDrive.reset(new TigerDrive(RobotMap::robotIMU.get()));

	//Configures the elevator talons
	//We want to use the relative encoder because we dont need absolute feedback
	//then we set the top of the elevator to zero i think..
	//sets PIDS
	elevatorClimberSubsystemPrimaryTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 10);
	elevatorClimberSubsystemPrimaryTalon->SetSelectedSensorPosition(TOP_POSITION_TICKS, 0, 10);
	elevatorClimberSubsystemPrimaryTalon->SetSensorPhase(false);
	elevatorClimberSubsystemPrimaryTalon->SetInverted(false);
	elevatorClimberSubsystemPrimaryTalon->Config_kP(0, 1, 10);
	elevatorClimberSubsystemPrimaryTalon->Config_kI(0, 0, 10);
	elevatorClimberSubsystemPrimaryTalon->Config_kD(0, 0, 10);

	//make the other motor follow the primary one
	elevatorClimberSubsystemFollower01Talon->Set(ControlMode::Follower, 13);
	elevatorClimberSubsystemFollower02Talon->SetInverted(false);

	elevatorClimberSubsystemFollower02Talon->Set(ControlMode::Follower, 13);
	elevatorClimberSubsystemFollower02Talon->SetInverted(false);

	//makes the drive talons drive the right way
	swerveSubsystemFLDriveTalon->SetInverted(false);
	swerveSubsystemFRDriveTalon->SetInverted(false);
	swerveSubsystemBLDriveTalon->SetInverted(false);
	swerveSubsystemBRDriveTalon->SetInverted(false);

	//zeros the quad encoders on start up
	swerveSubsystemFLDriveTalon->GetSensorCollection().SetQuadraturePosition(0, 10);
	swerveSubsystemFRDriveTalon->GetSensorCollection().SetQuadraturePosition(0, 10);
	swerveSubsystemBLDriveTalon->GetSensorCollection().SetQuadraturePosition(0, 10);
	swerveSubsystemBRDriveTalon->GetSensorCollection().SetQuadraturePosition(0, 10);

	//make sure we use the quad encoders when we do PID on them
	swerveSubsystemFLDriveTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
	swerveSubsystemFRDriveTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
	swerveSubsystemBLDriveTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);
	swerveSubsystemBRDriveTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::QuadEncoder, 0, 10);

	//for the talon PID to work we need to make sure that the sensors are in phase
	//this means that when we give the motor a positive voltage the encoder ticks MUST go more positive
	//if this is not set correctly PID will not work at all
	swerveSubsystemFLDriveTalon->SetSensorPhase(true);
	swerveSubsystemFRDriveTalon->SetSensorPhase(true);
	swerveSubsystemBLDriveTalon->SetSensorPhase(true);
	swerveSubsystemBRDriveTalon->SetSensorPhase(true);

	//we want to use relative for rotation motors because it is much faster read time
	swerveSubsystemFLRotTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 10);
	swerveSubsystemFRRotTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 10);
	swerveSubsystemBLRotTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 10);
	swerveSubsystemBRRotTalon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, 10);

	//we set the rotation sensor pos to the pulse width position masked. The masking makes sure we only are from 0 - 4096
	swerveSubsystemFLRotTalon->SetSelectedSensorPosition(swerveSubsystemFLRotTalon->GetSensorCollection().GetPulseWidthPosition() & 0xFFF, 0, 10);
	swerveSubsystemFRRotTalon->SetSelectedSensorPosition(swerveSubsystemFRRotTalon->GetSensorCollection().GetPulseWidthPosition() & 0xFFF, 0, 10);
	swerveSubsystemBLRotTalon->SetSelectedSensorPosition(swerveSubsystemBLRotTalon->GetSensorCollection().GetPulseWidthPosition() & 0xFFF, 0, 10);
	swerveSubsystemBRRotTalon->SetSelectedSensorPosition(swerveSubsystemBRRotTalon->GetSensorCollection().GetPulseWidthPosition() & 0xFFF, 0, 10);

	//Then we zero them out
	swerveSubsystemFLRotTalon->SetSelectedSensorPosition(0, 0, 10);
	swerveSubsystemFRRotTalon->SetSelectedSensorPosition(0, 0, 10);
	swerveSubsystemBLRotTalon->SetSelectedSensorPosition(0, 0, 10);
	swerveSubsystemBRRotTalon->SetSelectedSensorPosition(0, 0, 10);

	//see drive motor explanation above
	swerveSubsystemFLRotTalon->SetSensorPhase(false);
	swerveSubsystemFRRotTalon->SetSensorPhase(false);
	swerveSubsystemBLRotTalon->SetSensorPhase(false);
	swerveSubsystemBRRotTalon->SetSensorPhase(false);

	//makes sure we are rotating clockwise with positive voltage
	swerveSubsystemFLRotTalon->SetInverted(true);
	swerveSubsystemFRRotTalon->SetInverted(true);
	swerveSubsystemBLRotTalon->SetInverted(true);
	swerveSubsystemBRRotTalon->SetInverted(true);

	//PIDS
	swerveSubsystemFLRotTalon->Config_kP(0, 3.5, 10);
	swerveSubsystemFLRotTalon->Config_kI(0, 0, 10);
	swerveSubsystemFLRotTalon->Config_kD(0, .2, 10);

	swerveSubsystemFRRotTalon->Config_kP(0, 3.5, 10);
	swerveSubsystemFRRotTalon->Config_kI(0, 0, 10);
	swerveSubsystemFRRotTalon->Config_kD(0, .2, 10);

	swerveSubsystemBLRotTalon->Config_kP(0, 3.5, 10);
	swerveSubsystemBLRotTalon->Config_kI(0, 0, 10);
	swerveSubsystemBLRotTalon->Config_kD(0, .2, 10);

	swerveSubsystemBRRotTalon->Config_kP(0, 3.5, 10);
	swerveSubsystemBRRotTalon->Config_kI(0, 0, 10);
	swerveSubsystemBRRotTalon->Config_kD(0, .2, 10);

	//Rotation motors are too strong so we limit them to around half total output
	swerveSubsystemFLRotTalon->ConfigPeakOutputForward(.416, 10);
	swerveSubsystemFRRotTalon->ConfigPeakOutputForward(.416, 10);
	swerveSubsystemBLRotTalon->ConfigPeakOutputForward(.416, 10);
	swerveSubsystemBRRotTalon->ConfigPeakOutputForward(.416, 10);

	//same as above except in reverse
	swerveSubsystemFLRotTalon->ConfigPeakOutputReverse(-.416, 10);
	swerveSubsystemFRRotTalon->ConfigPeakOutputReverse(-.416, 10);
	swerveSubsystemBLRotTalon->ConfigPeakOutputReverse(-.416, 10);
	swerveSubsystemBRRotTalon->ConfigPeakOutputReverse(-.416, 10);

	//Tolerance for PID
	swerveSubsystemFLRotTalon->ConfigAllowableClosedloopError(0, 5, 10);
	swerveSubsystemFRRotTalon->ConfigAllowableClosedloopError(0, 5, 10);
	swerveSubsystemBLRotTalon->ConfigAllowableClosedloopError(0, 5, 10);
	swerveSubsystemBRRotTalon->ConfigAllowableClosedloopError(0, 5, 10);

	//This prevents the rotation motors from drawing too much current
	swerveSubsystemFLRotTalon->ConfigContinuousCurrentLimit(10, 10);
	swerveSubsystemFRRotTalon->ConfigContinuousCurrentLimit(10, 10);
	swerveSubsystemBLRotTalon->ConfigContinuousCurrentLimit(10, 10);
	swerveSubsystemBRRotTalon->ConfigContinuousCurrentLimit(10, 10);

	swerveSubsystemFLRotTalon->ConfigPeakCurrentLimit(0, 10);
	swerveSubsystemFRRotTalon->ConfigPeakCurrentLimit(0, 10);
	swerveSubsystemBLRotTalon->ConfigPeakCurrentLimit(0, 10);
	swerveSubsystemBRRotTalon->ConfigPeakCurrentLimit(0, 10);
}

